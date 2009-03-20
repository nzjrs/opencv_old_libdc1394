#include "_highgui.h"

#if defined HAVE_DC1394V2

#include <unistd.h>
#include <stdint.h>
#include <dc1394/dc1394.h>

#define DC1394_RETURN_IF_ERR(err, message)      \
    if (err != DC1394_SUCCESS) {                \
        dc1394_log_error(message);              \
        return false;                           \
    }                                           \

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
    bool        initFrame();
    bool        initDevice( int index );
    bool        shutdownDevice();
protected:
    dc1394camera_t      *m_camera;
    dc1394video_frame_t *m_frame;
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

bool CvCapture_DC1394V2::initFrame()
{
    dc1394error_t err;
    dc1394video_mode_t mode;
    unsigned int w, h;

    if (!m_camera)
        return false;

    err=dc1394_video_get_mode(m_camera, &mode);
    DC1394_RETURN_IF_ERR(err,"Failed get video mode");

    dc1394_get_image_size_from_video_mode(m_camera, mode, &w, &h);

    if (!m_image || (m_image && (m_image->width != w || m_image->height != h))) {
        if (m_image) {
            if (m_dc1394_owns_image) {
                cvReleaseImageHeader(&m_image);
            } else {
                cvReleaseImage(&m_image);
            }
        }

        //Setup the IPLImage to store the image data in
        m_image = cvCreateImageHeader(cvSize(w, h), IPL_DEPTH_8U, 1);
        if (!m_dc1394_owns_image)
            cvCreateData(&m_image);
    }

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
        dc1394camera_t *camera = dc1394_camera_new (dc1394, list->ids[0].guid);
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

    if (m_camera) {
        //Setup the camera in grey mode
        err=dc1394_camera_reset(m_camera);
        DC1394_RETURN_IF_ERR(err, "Could not reset camera");

        err=dc1394_video_set_mode(m_camera, DC1394_VIDEO_MODE_640x480_MONO8);
        DC1394_RETURN_IF_ERR(err,"Could not set video mode");

        err=dc1394_capture_setup(m_camera, 4, DC1394_CAPTURE_FLAGS_DEFAULT);
        DC1394_RETURN_IF_ERR(err,"Could not setup camera - make sure that the video mode is supported by your camera");

        err=dc1394_video_set_framerate(m_camera, DC1394_FRAMERATE_30);
        DC1394_RETURN_IF_ERR(err,"Could not set framerate");

        err=dc1394_video_set_transmission(m_camera, DC1394_ON);
        DC1394_RETURN_IF_ERR(err,"Could not start camera iso transmission");
    }
    
    m_initialized = initFrame();

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
