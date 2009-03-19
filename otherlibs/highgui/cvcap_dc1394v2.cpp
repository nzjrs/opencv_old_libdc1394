#include "_highgui.h"

#if !defined WIN32 && defined HAVE_DC1394V2

#include <unistd.h>
#include <stdint.h>
#include <dc1394/dc1394.h>

#ifdef NDEBUG
#define CV_WARN(message) 
#else
#define CV_WARN(message) fprintf(stderr, "warning: %s (%s:%d)\n", message, __FILE__, __LINE__)
#endif

#define CV_DC1394_CALL(expr)                                                  \
if((expr)<0){                                                                 \
	OPENCV_ERROR(CV_StsInternal, "", "libdc1394 function call returned < 0"); \
}

typedef struct __cvcap_dc1394_t
{
    dc1394_t            *d;
    dc1394camera_t      *camera;
    dc1394video_frame_t *frame;
//    int format;
//    int mode;
//    int color_mode;
//    int frame_rate;
//    char * device_name;
//    IplImage frame;
//	int convert;
//	int buffer_is_writeable;  // indicates whether frame.imageData is allocated by OpenCV or DC1394
}
cvcap_dc1394_t;

/*

static void icvCloseCAM_DC1394( cvcap_dc1394_t* capture );
static int icvGrabFrameCAM_DC1394( cvcap_dc1394_t* capture );
static IplImage* icvRetrieveFrameCAM_DC1394( cvcap_dc1394_t* capture );
static double icvGetPropertyCAM_DC1394( cvcap_dc1394_t* capture, int property_id );
static int    icvSetPropertyCAM_DC1394( cvcap_dc1394_t* capture, int property_id, double value );
static int    icvFormatSupportedCAM_DC1394(int format, quadlet_t formats);
static int    icvModeSupportedCAM_DC1394(int format, int mode, quadlet_t modes);
static int    icvColorMode( int mode );
static unsigned int icvGetBestFrameRate( cvcap_dc1394_t * capture, int format, int mode);
static int    icvResizeFrame(cvcap_dc1394_t * capture);

void icvInitCapture_DC1394(){
	int p;

	raw1394handle_t raw_handle = raw1394_new_handle();
	if( raw_handle == 0 ) {
		numPorts = 0;
		return;
	}
	numPorts = raw1394_get_port_info(raw_handle, ports, MAX_PORTS);
	raw1394_destroy_handle(raw_handle);
	for (p = 0; p < numPorts; p++) {
		handles[p] = dc1394_create_handle(p);
		if (handles[p]==NULL) {  numPorts=-1; return; }

		// get the camera nodes and describe them as we find them 
		camera_nodes = dc1394_get_camera_nodes(handles[p], &camCount[p], 0);
		for (int i=0;i<camCount[p];i++) {
			cameras[numCameras].cam.node = camera_nodes[i];
			cameras[numCameras].portnum = p;
			dc1394_stop_iso_transmission(handles[p], camera_nodes[i]);
			numCameras++;
		}
	}
};

static cvcap_dc1394_t * icvCaptureFromCAM_DC1394 (unsigned long long int guid)
{
	quadlet_t modes[8], formats;
	int i;

	if (numPorts<0) icvInitCapture_DC1394();
	if (numPorts==0)
		return 0;     //No i1394 ports found
	if (numCameras<1)
		return 0;
	if (index>=numCameras)
		return 0;
	if (index<0)
		return 0;

	cvcap_dc1394_t * pcap = (cvcap_dc1394_t*)cvAlloc(sizeof(*pcap));

	// Select a port and camera 
	pcap->device_name = videodev[cameras[index].portnum];
	pcap->handle = handles[cameras[index].portnum];
	pcap->camera = &cameras[index].cam;

	// get supported formats
	if (dc1394_query_supported_formats(pcap->handle, pcap->camera->node, &formats)<0) {
		fprintf(stderr,"%s:%d: Could not query supported formats\n",__FILE__,__LINE__);
		formats=0x0;
	}
	for (i=0; i < NUM_FORMATS; i++) {
		modes[i]=0;
		if (icvFormatSupportedCAM_DC1394(i+FORMAT_MIN, formats)){
			if (dc1394_query_supported_modes(pcap->handle, pcap->camera->node, i+FORMAT_MIN, &modes[i])<0) {
				fprintf(stderr,"%s:%d: Could not query Format%d modes\n",__FILE__,__LINE__,i);
			}
		}
	}

	pcap->format = 0;
	pcap->mode = 0;
	pcap->color_mode = 0;
	pcap->frame_rate = 0;

	int format_idx = -1;

	// scan the list of preferred modes, and find a supported one
	for(i=0; (pcap->mode == 0) && (preferred_modes[i] != 0); i++) {
		if((preferred_modes[i] >= FORMAT_MIN) && (preferred_modes[i] <= FORMAT_MAX)) {
			pcap->format = preferred_modes[i];
			format_idx = preferred_modes[i] - FORMAT_MIN;
			continue;
		}
		assert(format_idx != -1);
		if ( ! icvFormatSupportedCAM_DC1394(pcap->format, formats) )
			continue;
		if ( icvModeSupportedCAM_DC1394(pcap->format, preferred_modes[i], modes[format_idx]) ){
			pcap->mode = preferred_modes[i];
		}
	}
	if (pcap->mode == 0) {
		fprintf(stderr,"%s:%d: Could not find a supported mode for this camera\n",__FILE__,__LINE__);
		goto ERROR;
	}

	pcap->color_mode = icvColorMode( pcap->mode );
	if( pcap->color_mode == -1){
		fprintf(stderr,"%s:%d: ERROR: BPP is Unsupported!!\n",__FILE__,__LINE__);
		goto ERROR;
	}

	// set frame rate to optimal value given format and mode
	pcap->frame_rate = icvGetBestFrameRate(pcap, pcap->format, pcap->mode);

	if (pcap->format!=FORMAT_SCALABLE_IMAGE_SIZE) { // everything except Format 7
		if (dc1394_dma_setup_capture(pcap->handle, pcap->camera->node, index+1 ,
					pcap->format, pcap->mode, SPEED_400, 
					pcap->frame_rate, NUM_BUFFERS,
#ifdef HAVE_DC1394_095
					0 ,
#endif
					1 ,
					pcap->device_name, pcap->camera) != DC1394_SUCCESS) {
			fprintf(stderr,"%s:%d: Failed to setup DMA capture with VIDEO1394\n",__FILE__,__LINE__);
			goto ERROR;
		}
	}
	else {
		if(dc1394_dma_setup_format7_capture(pcap->handle,pcap->camera->node,index+1 ,
					pcap->mode, SPEED_400, QUERY_FROM_CAMERA,
					(unsigned int)QUERY_FROM_CAMERA, (unsigned int)QUERY_FROM_CAMERA,
					(unsigned int)QUERY_FROM_CAMERA, (unsigned int)QUERY_FROM_CAMERA,
					NUM_BUFFERS,
#ifdef HAVE_DC1394_095
					0 ,
#endif
					1 ,
					pcap->device_name, pcap->camera) != DC1394_SUCCESS) {
			fprintf(stderr,"%s:%d: Failed to setup DMA capture with VIDEO1394\n",__FILE__,__LINE__);
			goto ERROR;
		}
	}

	if (dc1394_start_iso_transmission(pcap->handle, pcap->camera->node)!=DC1394_SUCCESS) {
		fprintf(stderr,"%s:%d: Could not start ISO transmission\n",__FILE__,__LINE__);
		goto ERROR;
	}

	usleep(DELAY);

	dc1394bool_t status;
	if (dc1394_get_iso_status(pcap->handle, pcap->camera->node, &status)!=DC1394_SUCCESS) {
		fprintf(stderr,"%s:%d: Could get ISO status",__FILE__,__LINE__);
		goto ERROR;
	}
	if (status==DC1394_FALSE) {
		fprintf(stderr,"%s:%d: ISO transmission refuses to start",__FILE__,__LINE__);
		goto ERROR;
	}

	// convert camera image to RGB by default
	pcap->convert=1;

	// no image data allocated yet
	pcap->buffer_is_writeable = 0;

	memset(&(pcap->frame), 0, sizeof(IplImage));
	icvResizeFrame( pcap );
	return pcap;

ERROR:
	return 0;  
};

static void icvCloseCAM_DC1394( cvcap_dc1394_t* capture ){
	dc1394_stop_iso_transmission(capture->handle, capture->camera->node);
	dc1394_dma_unlisten (capture->handle, capture->camera);
	//Deallocate space for RGBA data
	if(capture->convert){
		cvFree(&capture->frame.imageData);
	}
}

static int icvGrabFrameCAM_DC1394( cvcap_dc1394_t* capture ){
	// TODO: should this function wait until the next frame is available or return
	// immediately ?
	float waiting = 0;
	do{
		int result = dc1394_dma_single_capture_poll(capture->camera);
		if(result==DC1394_SUCCESS){
			return 1;
		}
		else if(result==DC1394_NO_FRAME){
			usleep(1000000/120);  //sleep for at least a 1/2 of the frame rate
			waiting += 1.0/120.0;
		}
		else{
			printf("dc1394_dma_single_capture_poll failed\n");
			return 0;
		}
	} while(waiting<2);
	printf("dc1394_dma_single_capture_poll timed out\n");
	return 0;
}

static IplImage* icvRetrieveFrameCAM_DC1394( cvcap_dc1394_t* capture ){
	if(capture->camera->capture_buffer )
	{
		if(capture->convert){
			//Convert to RGBA
			unsigned char * src = (unsigned char *)capture->camera->capture_buffer;
			unsigned char * dst = (unsigned char *)capture->frame.imageData;
			switch (capture->color_mode) {
				case COLOR_FORMAT7_RGB8:
					//printf("icvRetrieveFrame convert RGB to BGR\n");
					//Convert RGB to BGR
					for (int i=0;i<capture->frame.imageSize;i+=6) {
						dst[i]   = src[i+2];
						dst[i+1] = src[i+1];
						dst[i+2] = src[i];
						dst[i+3] = src[i+5];
						dst[i+4] = src[i+4];
						dst[i+5] = src[i+3];
					}
					break;
				case COLOR_FORMAT7_YUV422:
					//printf("icvRetrieveFrame convert YUV422 to BGR %d\n");
					uyvy2bgr(src,
							dst,
							capture->camera->frame_width * capture->camera->frame_height);
					break;
				case COLOR_FORMAT7_MONO8:
					//printf("icvRetrieveFrame convert MONO8 to BGR %d\n");
					y2bgr(src,
							dst,
							capture->camera->frame_width * capture->camera->frame_height);
					break;
				case COLOR_FORMAT7_YUV411:
					//printf("icvRetrieveFrame convert YUV411 to BGR %d\n");
					uyyvyy2bgr(src,
							dst,
							capture->camera->frame_width * capture->camera->frame_height);
					break;
				case COLOR_FORMAT7_YUV444:
					//printf("icvRetrieveFrame convert YUV444 to BGR %d\n");
					uyv2bgr(src,
							dst,
							capture->camera->frame_width * capture->camera->frame_height);
					break;
				case COLOR_FORMAT7_MONO16:
					//printf("icvRetrieveFrame convert MONO16 to BGR %d\n");
					y162bgr(src,
							dst,
							capture->camera->frame_width * capture->camera->frame_height, MONO16_BPP);
					break;
				case COLOR_FORMAT7_RGB16:
					//printf("icvRetrieveFrame convert RGB16 to BGR %d\n");
					rgb482bgr(src,
							dst,
							capture->camera->frame_width * capture->camera->frame_height, MONO16_BPP);
					break;
				default:
					fprintf(stderr,"%s:%d: Unsupported color mode %d\n",__FILE__,__LINE__,capture->color_mode);
					return 0;
			} //switch (capture->mode)
		}
		else{
			// return raw data
			capture->frame.imageData = (char *) capture->camera->capture_buffer;
			capture->frame.imageDataOrigin = (char *) capture->camera->capture_buffer;
		}
		
		// TODO: if convert=0, we are not actually done with the buffer
		// but this seems to work anyway.
		dc1394_dma_done_with_buffer(capture->camera);
		
		return &capture->frame;
	}
	return 0;
};

static double icvGetPropertyCAM_DC1394( cvcap_dc1394_t* capture, int property_id ){
	int index=-1;	
	switch ( property_id ) {
		case CV_CAP_PROP_CONVERT_RGB:
			return capture->convert;
		case CV_CAP_PROP_MODE:
			return capture->mode;
		case CV_CAP_PROP_FORMAT:
			return capture->format;
		case CV_CAP_PROP_FPS:
			CV_DC1394_CALL(dc1394_get_video_framerate(capture->handle, capture->camera->node,
					(unsigned int *) &capture->camera->frame_rate));
			switch(capture->camera->frame_rate) {
				case FRAMERATE_1_875:
					return 1.875;
				case FRAMERATE_3_75:
					return 3.75;
				case FRAMERATE_7_5:
					return 7.5;
				case FRAMERATE_15:
					return 15.;
				case FRAMERATE_30:
					return 30.;
				case FRAMERATE_60:
					return 60;
#if NUM_FRAMERATES > 6
				case FRAMERATE_120:
					return 120;
#endif
#if NUM_FRAMERATES > 7
				case FRAMERATE_240:
					return 240;
#endif
			}
        default:    
			index = property_id;  // did they pass in a LIBDC1394 feature flag?
			break;
	}
	if(index>=FEATURE_MIN && index<=FEATURE_MAX){
		dc1394bool_t has_feature;
		CV_DC1394_CALL( dc1394_is_feature_present(capture->handle, capture->camera->node, 
					                              index, &has_feature));
		if(!has_feature){
			CV_WARN("Feature is not supported by this camera");
		}
		else{
			unsigned int value;
			dc1394_get_feature_value(capture->handle, capture->camera->node, index, &value);
			return (double) value;
		}
	}

	return 0;
};

// resize capture->frame appropriately depending on camera and capture settings
static int icvResizeFrame(cvcap_dc1394_t * capture){
	if(capture->convert){
		// resize if sizes are different, formats are different
		// or conversion option has changed
		if(capture->camera->frame_width != capture->frame.width ||
		   capture->camera->frame_height != capture->frame.height ||
		   capture->frame.depth != 8 ||
		   capture->frame.nChannels != 3 ||
		   capture->frame.imageData == NULL ||
		   capture->buffer_is_writeable == 0) 
		{
			if(capture->frame.imageData && capture->buffer_is_writeable){ 
				cvReleaseData( &(capture->frame));
			}
			cvInitImageHeader( &capture->frame, cvSize( capture->camera->frame_width,
						                                capture->camera->frame_height ),
								IPL_DEPTH_8U, 3, IPL_ORIGIN_TL, 4 );
			cvCreateData( &(capture->frame) );
			capture->buffer_is_writeable = 1;
		}

	}
	else {
		// free image data if allocated by opencv
		if(capture->buffer_is_writeable){
			cvReleaseData(&(capture->frame));
		}

		// figure out number of channels and bpp
		int bpp = 8;
		int nch = 3;
		int width = capture->camera->frame_width;
		int height = capture->camera->frame_height;
		double code = CV_FOURCC('B','G','R',0);
		switch(capture->color_mode){
		case COLOR_FORMAT7_YUV422:
			nch = 2;
			code = CV_FOURCC('Y','4','2','2');
			break;
		case COLOR_FORMAT7_MONO8:
			code = CV_FOURCC('Y',0,0,0);
			nch = 1;
			break;
		case COLOR_FORMAT7_YUV411:
			code = CV_FOURCC('Y','4','1','1');
			width *= 2;
			nch = 3;  //yy[u/v]
			break;
		case COLOR_FORMAT7_YUV444:
			code = CV_FOURCC('Y','U','V',0);
			nch = 3;
			break;
		case COLOR_FORMAT7_MONO16:
			code = CV_FOURCC('Y',0,0,0);
			bpp = IPL_DEPTH_16S;
			nch = 1;
			break;
		case COLOR_FORMAT7_RGB16:
			bpp = IPL_DEPTH_16S;
			nch = 3;
			break;
		default:
			break;
		}
		// reset image header
		cvInitImageHeader( &capture->frame,cvSize( width, height ), bpp, nch, IPL_ORIGIN_TL, 4 );
		//assert(capture->frame.imageSize == capture->camera->quadlets_per_frame*4);
		capture->buffer_is_writeable = 0;
	}
	return 1;
}

// Toggle setting about whether or not RGB color conversion is to be performed
// Allocates/Initializes capture->frame appropriately
int icvSetConvertRGB(cvcap_dc1394_t * capture, int convert){
	if(convert==capture->convert){
		// no action necessary
		return 1;
	}
	capture->convert = convert;
	return icvResizeFrame( capture );
}

// given desired format, mode, and modes bitmask from camera, determine if format and mode are supported
static int
icvFormatSupportedCAM_DC1394(int format, quadlet_t formats){
	// formats is a bitmask whose higher order bits indicate whether format is supported
	int shift = 31 - (format - FORMAT_MIN);
	int mask = 1 << shift;
	return (formats & mask) != 0;
}

// analyze modes bitmask from camera to determine if desired format and mode are supported
static int
icvModeSupportedCAM_DC1394(int format, int mode, quadlet_t modes){
	// modes is a bitmask whose higher order bits indicate whether mode is supported
	int format_idx = format - FORMAT_MIN;
	int mode_format_min = MODE_FORMAT0_MIN + 32*format_idx;
	int shift = 31 - (mode - mode_format_min);
	int mask = 0x1 << shift; 
	return (modes & mask) != 0;
}

// Setup camera to use given dc1394 mode
static int
icvSetModeCAM_DC1394( cvcap_dc1394_t * capture, int mode ){
	quadlet_t modes, formats;
	//printf("<icvSetModeCAM_DC1394>\n");

	// figure out corrent format for this mode
	int format = (mode - MODE_FORMAT0_MIN) / 32 + FORMAT_MIN;

	// get supported formats
	if (dc1394_query_supported_formats(capture->handle, capture->camera->node, &formats)<0) {
		fprintf(stderr,"%s:%d: Could not query supported formats\n",__FILE__,__LINE__);
		return 0;
	}

	// is format for requested mode supported ?
	if(icvFormatSupportedCAM_DC1394(format, formats)==0){
		return 0;
	}

	// get supported modes for requested format
	if (dc1394_query_supported_modes(capture->handle, capture->camera->node, format, &modes)<0){
		fprintf(stderr,"%s:%d: Could not query supported modes for format %d\n",__FILE__,__LINE__, capture->format);
		return 0;
	}

	// is requested mode supported ?
	if(! icvModeSupportedCAM_DC1394(format, mode, modes) ){
		return 0;
	}

	int color_mode = icvColorMode( mode );

	if(color_mode == -1){
		return 0;
	}

	int frame_rate = icvGetBestFrameRate(capture, format, mode);

	dc1394_dma_unlisten(capture->handle, capture->camera);
	if (dc1394_dma_setup_capture(capture->handle, capture->camera->node, capture->camera->channel,
				format, mode, SPEED_400, 
				frame_rate, NUM_BUFFERS,
#ifdef HAVE_DC1394_095
				0,
#endif
				1,
				capture->device_name, capture->camera) != DC1394_SUCCESS) {
		fprintf(stderr,"%s:%d: Failed to setup DMA capture with VIDEO1394\n",__FILE__,__LINE__);
		return 0;
	}
	dc1394_start_iso_transmission(capture->handle, capture->camera->node);

	capture->frame_rate = frame_rate;
	capture->format = format;
	capture->mode = mode;
	capture->color_mode = color_mode;

	// now fix image size to match new mode
	icvResizeFrame( capture );
	return 1;
}

// query camera for supported frame rates and select fastest for given format and mode
static unsigned int icvGetBestFrameRate( cvcap_dc1394_t * capture, int format, int mode  ){
	quadlet_t framerates;
	if (dc1394_query_supported_framerates(capture->handle, capture->camera->node, 
				format, mode, &framerates)!=DC1394_SUCCESS) 
	{
		fprintf(stderr,"%s:%d: Could not query supported framerates\n",__FILE__,__LINE__);
		framerates = 0;
	}

	for (int f=FRAMERATE_MAX; f>=FRAMERATE_MIN; f--) {
		if (framerates & (0x1<< (31-(f-FRAMERATE_MIN)))) {
			return f;
		}
	}
	return 0;
}

static int
icvSetFrameRateCAM_DC1394( cvcap_dc1394_t * capture, double value ){
	unsigned int fps=15;
	if(capture->format == FORMAT_SCALABLE_IMAGE_SIZE)
		return 0; // format 7 has no fixed framerates
	if (value==-1){ 
		fps=icvGetBestFrameRate( capture, capture->format, capture->mode );
	}
	else if (value==1.875)
		fps=FRAMERATE_1_875;
	else if (value==3.75)
		fps=FRAMERATE_3_75;
	else if (value==7.5)
		fps=FRAMERATE_7_5;
	else if (value==15)
		fps=FRAMERATE_15;
	else if (value==30)
		fps=FRAMERATE_30;
	else if (value==60)
		fps=FRAMERATE_60;
#if NUM_FRAMERATES > 6
	else if (value==120)
		fps=FRAMERATE_120;
#endif
#if NUM_FRAMERATES > 7
	else if (value==240)
		fps=FRAMERATE_240;
#endif
	dc1394_set_video_framerate(capture->handle, capture->camera->node,fps);
	dc1394_get_video_framerate(capture->handle, capture->camera->node,
					(unsigned int *) &capture->camera->frame_rate);

	return fps==(unsigned int) capture->camera->frame_rate;
}

// for given mode return color format
static int
icvColorMode( int mode ){
	switch(mode) {
	case MODE_160x120_YUV444:
		return COLOR_FORMAT7_YUV444;
	case MODE_320x240_YUV422:
	case MODE_640x480_YUV422:
	case MODE_800x600_YUV422:
	case MODE_1024x768_YUV422:
	case MODE_1280x960_YUV422:
	case MODE_1600x1200_YUV422:
		return COLOR_FORMAT7_YUV422;
	case MODE_640x480_YUV411:
		return COLOR_FORMAT7_YUV411;
	case MODE_640x480_RGB:
	case MODE_800x600_RGB:
	case MODE_1024x768_RGB:
	case MODE_1280x960_RGB:
	case MODE_1600x1200_RGB:
		return COLOR_FORMAT7_RGB8;
	case MODE_640x480_MONO:
	case MODE_800x600_MONO:
	case MODE_1024x768_MONO:
	case MODE_1280x960_MONO:
	case MODE_1600x1200_MONO:
		return COLOR_FORMAT7_MONO8;
	case MODE_640x480_MONO16:
	case MODE_800x600_MONO16:
	case MODE_1024x768_MONO16:
	case MODE_1280x960_MONO16:
	case MODE_1600x1200_MONO16:
		return COLOR_FORMAT7_MONO16;
	case MODE_FORMAT7_0:
	case MODE_FORMAT7_1:
	case MODE_FORMAT7_2:
	case MODE_FORMAT7_3:
	case MODE_FORMAT7_4:
	case MODE_FORMAT7_5:
	case MODE_FORMAT7_6:
	case MODE_FORMAT7_7:
		fprintf(stderr,"%s:%d: Format7 not yet supported\n",__FILE__,__LINE__);
	default:
		break;
	}
	return -1;
}

// function to set camera properties using dc1394 feature enum
// val == -1 indicates to set this property to 'auto'
static int
icvSetFeatureCAM_DC1394( cvcap_dc1394_t* capture, int feature_id, int val){
        dc1394bool_t isOn = DC1394_FALSE;
		dc1394bool_t hasAutoCapability = DC1394_FALSE;
		dc1394bool_t isAutoOn = DC1394_FALSE;
		unsigned int nval;
		unsigned int minval,maxval;

		// Turn the feature on if it is OFF
		if( dc1394_is_feature_on(capture->handle, capture->camera->node, feature_id, &isOn) 
				== DC1394_FAILURE ) {
			return 0;
		}
		if( isOn == DC1394_FALSE ) {
                // try to turn it on.
                if( dc1394_feature_on_off(capture->handle, capture->camera->node, feature_id, 1) == DC1394_FAILURE ) {
                    fprintf(stderr, "error turning feature %d on!\n", feature_id);
                    return 0;
                }
		}

		// Check if the feature supports auto mode 
		dc1394_has_auto_mode(capture->handle, capture->camera->node, feature_id, &hasAutoCapability);
		if( hasAutoCapability ) {

			// now check if the auto is on.
			if( dc1394_is_feature_auto(capture->handle, capture->camera->node, feature_id, &isAutoOn ) == DC1394_FAILURE ) {
				fprintf(stderr, "error determining if feature %d has auto on!\n", index);
				return 0;
			}
		}
		// Caller requested auto mode, but cannot support it
		else if(val==-1){
            fprintf(stderr, "feature %d does not support auto mode\n", feature_id);
            return 0;
		}

		if(val==-1){
            // if the auto mode isn't enabled, enable it
            if( isAutoOn == DC1394_FALSE ) {
                if(dc1394_auto_on_off(capture->handle, capture->camera->node, feature_id, 1) == DC1394_FAILURE ) {
                    fprintf(stderr, "error turning feature %d auto ON!\n", feature_id);
                    return 0;
                }
            }
			return 1;
        }

		// ELSE turn OFF auto and adjust feature manually
		if( isAutoOn == DC1394_TRUE ) {
			if(dc1394_auto_on_off(capture->handle, capture->camera->node, feature_id, 0) == DC1394_FAILURE ) {
				fprintf(stderr, "error turning feature %d auto OFF!\n", feature_id);
				return 0;
			}
		}

		// Clamp val to within feature range
		CV_DC1394_CALL(	dc1394_get_min_value(capture->handle, capture->camera->node, feature_id, &minval));
		CV_DC1394_CALL(	dc1394_get_max_value(capture->handle, capture->camera->node, feature_id, &maxval));
		val = MIN(maxval, MAX(val, minval));


		if (dc1394_set_feature_value(capture->handle, capture->camera->node, feature_id, val) ==
				DC1394_FAILURE){
			fprintf(stderr, "error setting feature value\n");
			return 0;
		}
		if (dc1394_get_feature_value(capture->handle, capture->camera->node, feature_id, &nval) ==
				DC1394_FAILURE){
			fprintf(stderr, "error setting feature value\n");
			return 0;
		}
		return nval==(unsigned int)val;

}

// cvSetCaptureProperty callback function implementation
static int   
icvSetPropertyCAM_DC1394( cvcap_dc1394_t* capture, int property_id, double value ){
	int index=-1;
	switch ( property_id ) {
		case CV_CAP_PROP_CONVERT_RGB:
			return icvSetConvertRGB( capture, value != 0 );
		case CV_CAP_PROP_MODE:
			return icvSetModeCAM_DC1394( capture, (int) value );
		case CV_CAP_PROP_FPS:
			return icvSetFrameRateCAM_DC1394( capture, value );
		case CV_CAP_PROP_BRIGHTNESS:
			index = FEATURE_BRIGHTNESS;
			break;
		case CV_CAP_PROP_CONTRAST: 
			index = FEATURE_GAMMA;
			break;
		case CV_CAP_PROP_SATURATION: 
			index = FEATURE_SATURATION;
			break;
		case CV_CAP_PROP_HUE:       
			index = FEATURE_HUE;
			break;
		case CV_CAP_PROP_GAIN:     
			index = FEATURE_GAIN;
			break;
		default:
			index = property_id;  // did they pass in a LIBDC1394 feature flag?
			break;
	}
	if(index>=FEATURE_MIN && index<=FEATURE_MAX){
		return icvSetFeatureCAM_DC1394(capture, index, (int) value);
	}
	return 0;
};

*/

class CvCaptureCAM_DC1394V2_CPP : public CvCapture
{
public:
    CvCaptureCAM_DC1394V2_CPP() { captureDC1394 = 0; }
    virtual ~CvCaptureCAM_DC1394V2_CPP() { close(); }

    virtual bool open( unsigned long long int guid );
    virtual void close();

    virtual double getProperty(int);
    virtual bool setProperty(int, double);
    virtual bool grabFrame();
    virtual IplImage* retrieveFrame();
protected:

    cvcap_dc1394_t *captureDC1394;
};

bool CvCaptureCAM_DC1394V2_CPP::open( unsigned long long int guid )
{
//    close();
//    captureDC1394 = icvCaptureFromCAM_DC1394(guid);
//    return captureDC1394 != 0;
    return false;
}

void CvCaptureCAM_DC1394V2_CPP::close()
{
    ;
//    if( captureDC1394 )
//    {
//        icvCloseCAM_DC1394( captureDC1394 );
//        cvFree( &captureDC1394 );
//    }
}

bool CvCaptureCAM_DC1394V2_CPP::grabFrame()
{
    return false;
//    return captureDC1394 ? icvGrabFrameCAM_DC1394( captureDC1394 ) != 0 : false;
}

IplImage* CvCaptureCAM_DC1394V2_CPP::retrieveFrame()
{
    return 0;
//    return captureDC1394 ? (IplImage*)icvRetrieveFrameCAM_DC1394( captureDC1394 ) : 0;
}

double CvCaptureCAM_DC1394V2_CPP::getProperty( int propId )
{
    return 0;
//    return captureDC1394 ? icvGetPropertyCAM_DC1394( captureDC1394, propId ) : 0;
}

bool CvCaptureCAM_DC1394V2_CPP::setProperty( int propId, double value )
{
    return false;
//    return captureDC1394 ? icvSetPropertyCAM_DC1394( captureDC1394, propId, value ) != 0 : false;
}

CvCapture* cvCreateCameraCapture_DC1394( unsigned long long int guid )
{
    return 0;
//    CvCaptureCAM_DC1394V2_CPP* capture = new CvCaptureCAM_DC1394V2_CPP;
//
//    if( capture->open( guid ))
//        return capture;
//
//    delete capture;
//    return 0;
}

#endif
