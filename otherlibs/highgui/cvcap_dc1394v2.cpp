#include "_highgui.h"

#if defined HAVE_DC1394V2

#include <cv.h>
#include <unistd.h>
#include <stdint.h>
#include <dc1394/dc1394.h>

#define DC1394_RETURN_IF_ERR(err, message)      \
    if (err != DC1394_SUCCESS) {                \
        dc1394_log_error(message);              \
        return false;                           \
    }                                           \

#define DEFAULT_VIDEO_MODE      DC1394_VIDEO_MODE_640x480_MONO8
#define DEFAULT_FRAMERATE       DC1394_FRAMERATE_30
#define DEFAULT_COLOR_CODING    DC1394_COLOR_CODING_RAW8

#define USE_COLOUR              0

static unsigned int num_cameras = 0;
static dc1394_t *dc1394 = 0;

class CvCapture_DC1394V2 : public CvCapture
{
public:
    CvCapture_DC1394V2();
    ~CvCapture_DC1394V2();
    bool        open( int );
    void        close();
    double      getProperty(int);
    bool        setProperty(int, double);
    bool        grabFrame();
    IplImage*   retrieveFrame();
private:
    bool        initFrame(dc1394video_mode_t video_mode, dc1394color_coding_t color_coding);
    bool        initDevice( int index );
    bool        shutdownDevice();
protected:
    dc1394camera_t      *m_camera;
    dc1394video_frame_t *m_frame;
    dc1394video_mode_t  m_video_mode;
    IplImage            *m_image;
    bool                m_dc1394_owns_image;
    bool                m_initialized;
};

CvCapture_DC1394V2::CvCapture_DC1394V2() :
    m_camera(0),
    m_frame(0),
    m_image(0),
    m_dc1394_owns_image(true),
    m_initialized(false)
{
    if (!dc1394)
        dc1394 = dc1394_new();
    num_cameras += 1;
}

CvCapture_DC1394V2::~CvCapture_DC1394V2()
{
    close();
    if (--num_cameras == 0) {
        dc1394_free(dc1394);
        dc1394 = 0;
    }
}

bool CvCapture_DC1394V2::shutdownDevice()
{
    if (m_dc1394_owns_image) {
        cvReleaseImageHeader(&m_image);
    } else {
        cvReleaseImage(&m_image);
    }

    dc1394_video_set_transmission(m_camera, DC1394_OFF);
    dc1394_capture_stop(m_camera);
    dc1394_camera_free(m_camera);

    m_camera = 0;
    m_frame = 0;
    m_image = 0;
    m_initialized = false;

    return m_initialized;
}

bool CvCapture_DC1394V2::initFrame(dc1394video_mode_t video_mode, dc1394color_coding_t color_coding)
{
    dc1394error_t err;
    unsigned int w, h;
    int nchannels;

    if (!m_camera)
        return false;

    err=dc1394_camera_reset(m_camera);
    DC1394_RETURN_IF_ERR(err, "Could not reset camera");

    err = dc1394_video_set_iso_speed(m_camera, DC1394_ISO_SPEED_200);
    DC1394_RETURN_IF_ERR(err,"Could not setup camera ISO speed");

    dc1394_get_image_size_from_video_mode(m_camera, video_mode, &w, &h);

    if ( video_mode == DC1394_VIDEO_MODE_640x480_MONO8 ) {
        //Setup the camera in grey mode
        err=dc1394_video_set_mode(m_camera, video_mode);
        DC1394_RETURN_IF_ERR(err,"Could not set video mode");

        err=dc1394_capture_setup(m_camera, 4, DC1394_CAPTURE_FLAGS_DEFAULT);
        DC1394_RETURN_IF_ERR(err,"Could not setup camera - make sure that the video mode is supported by your camera");

    } else if ( (video_mode == DC1394_VIDEO_MODE_FORMAT7_0) ||
                (video_mode == DC1394_VIDEO_MODE_FORMAT7_1)) {
        unsigned int packet_size;

        err=dc1394_format7_get_recommended_packet_size (m_camera, video_mode, &packet_size);
        DC1394_RETURN_IF_ERR(err,"Could not get recommended packet size");

        if (video_mode == DC1394_VIDEO_MODE_FORMAT7_0) {
            err=dc1394_format7_set_roi(
                    m_camera,
                    video_mode,
                    color_coding,
                    packet_size,
                    0, 0,
                    w,
                    h);
            DC1394_RETURN_IF_ERR(err,"Could not set roi");
        }

        err=dc1394_video_set_mode(m_camera, video_mode);
        DC1394_RETURN_IF_ERR(err,"Could not set video mode");

        err=dc1394_capture_setup(m_camera, 4, DC1394_CAPTURE_FLAGS_DEFAULT);
        DC1394_RETURN_IF_ERR(err,"Could not setup camera-\nmake sure that the video mode and framerate are\nsupported by your camera");
    } else {
        dc1394_log_error("Invalid video mode");
        return false;
    }

    err=dc1394_video_set_framerate(m_camera, DEFAULT_FRAMERATE);
    DC1394_RETURN_IF_ERR(err,"Could not set framerate");

    err=dc1394_video_set_transmission(m_camera, DC1394_ON);
    DC1394_RETURN_IF_ERR(err,"Could not start camera iso transmission");

    // Free the old frame resources
    if (!m_image || (m_image && (m_image->width != w || m_image->height != h))) {
        if (m_image) {
            if (m_dc1394_owns_image) {
                cvReleaseImageHeader(&m_image);
            } else {
                cvReleaseImage(&m_image);
            }
        }

        if ( video_mode == DC1394_VIDEO_MODE_640x480_MONO8 ) {
            // If in grey mode, he image data returned from DC1394 is the 
            // same packing as that in IplImage, so we dont need to memcpy, 
            // just point to the underlying dc1394_video_frame
            m_dc1394_owns_image = true;
            nchannels = 1;
        } else {
            // Else we need to debayer the image
            m_dc1394_owns_image = false;
            nchannels = 3;
        }

        // Setup the IPLImage to store the image data in
        m_image = cvCreateImageHeader(cvSize(w, h), IPL_DEPTH_8U, nchannels);
        if (!m_dc1394_owns_image)
            cvCreateImageData(m_image);
    }

    m_video_mode = video_mode;
    return true;
}

bool CvCapture_DC1394V2::initDevice( int index )
{
    unsigned int i;
    dc1394camera_list_t * list;
    dc1394error_t err;

    err=dc1394_camera_enumerate (dc1394, &list);
    DC1394_RETURN_IF_ERR(err,"Failed to enumerate cameras");

    if (list->num == 0) {
        dc1394_log_error("No cameras found");
    }

    //Find the selected camera
    for (i = 0; i < list->num; i++) {
        dc1394camera_t *camera = dc1394_camera_new (dc1394, list->ids[i].guid);
        if (camera) {
            dc1394_camera_print_info(camera, stdout);
            if (i == index) {
                m_camera = camera;
            }
            else
                dc1394_camera_free(camera);
        }
    }
    dc1394_camera_free_list (list);

#if USE_COLOUR
    // Color (640x480, F7)
    m_initialized = initFrame(DC1394_VIDEO_MODE_FORMAT7_0, DC1394_COLOR_CODING_RAW8);
#else
    // B&W
    m_initialized = initFrame(DEFAULT_VIDEO_MODE, DEFAULT_COLOR_CODING);
#endif

    return m_initialized;
}

bool CvCapture_DC1394V2::open( int index )
{
    if (m_initialized)
        close();
    return initDevice(index);
}

void CvCapture_DC1394V2::close()
{    
    if (m_initialized)
        shutdownDevice();
}

bool CvCapture_DC1394V2::grabFrame()
{
    dc1394error_t err;

    err=dc1394_capture_dequeue(m_camera, DC1394_CAPTURE_POLICY_WAIT, &m_frame);
    DC1394_RETURN_IF_ERR(err,"Could not capture a frame");

    err=dc1394_capture_enqueue(m_camera, m_frame);
    DC1394_RETURN_IF_ERR(err,"releasing buffer");

    return true;
}

IplImage* CvCapture_DC1394V2::retrieveFrame()
{
    if (m_dc1394_owns_image) {
    	m_image->imageData = (char *) m_frame->image;
    	m_image->imageDataOrigin = (char *) m_frame->image;
    } else {
        if ((m_video_mode == DC1394_VIDEO_MODE_FORMAT7_0) ||
            (m_video_mode == DC1394_VIDEO_MODE_FORMAT7_1)) {
            dc1394error_t err;
            dc1394video_frame_t dest;
            IplImage *tmp;
            unsigned char *imdata;

            // debayer frame into RGB8
            imdata = (unsigned char *)malloc(m_frame->size[0]*m_frame->size[1]*3*sizeof(unsigned char));
            dest.image = imdata;
            dest.color_coding = DC1394_COLOR_CODING_RGB8;
            err=dc1394_debayer_frames(m_frame, &dest, DC1394_BAYER_METHOD_NEAREST); 
            if (err != DC1394_SUCCESS)
                dc1394_log_error("Could not convert/debayer frames");

            // convert from RGB to BGR
            tmp = cvCreateImageHeader(cvSize(m_frame->size[0], m_frame->size[1]), IPL_DEPTH_8U, 3);
            cvSetData(tmp, imdata, m_frame->size[0]*3);
            cvCvtColor(tmp, m_image, CV_RGB2BGR);

            free(imdata);
            cvReleaseImageHeader(&tmp);
        }
    }
    return m_image;
}

double CvCapture_DC1394V2::getProperty( int propId )
{
    return 0;
}

bool CvCapture_DC1394V2::setProperty( int propId, double value )
{
    return false;
}

CvCapture* cvCreateCameraCapture_DC1394V2( int index )
{
    CvCapture_DC1394V2* capture = new CvCapture_DC1394V2;
    if( capture->open( index ))
        return capture;
    delete capture;
    return 0;
}

#endif
