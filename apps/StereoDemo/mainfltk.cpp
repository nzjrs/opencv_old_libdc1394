/*
 * compile on Unix using:
 gcc `pkg-config --cflags --libs opencv` \
    `fltk-config --cxxflags --ldflags --libs` -DFLTK \
    mainfltk.cpp  main.cpp -o stereodemofltk
*/
#ifdef FLTK
#include "cvaux.h"
#include "cvfltk.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "highgui.h"


//********************** Calibration parameters **************************
extern CvCalibFilter CalibFilter;
extern double EtalonParams[3];
extern int prev_time;
//************************************************************************


typedef struct 
{
    char* name;
    int type;
    int minimum;
    int maximum;
    int step;
    void* value;
} Parameter;

#define TYPE_RANGE 1
#define TYPE_BOOL 2

/*parameters*/
int max_disp;

Parameter BirchfieldParams[] = { {"maxdisp", TYPE_RANGE, 1, 150, 1, &max_disp},
                                 0 };   


char* methods_names[] = {"Modified Birchfield", 0 };
Parameter* methods_params[] = {BirchfieldParams, 0};
                                                                 
class VideoWindow;

Fl_Window *root_window=(Fl_Window *)0;
VideoWindow* video_window_left = 0;
VideoWindow* video_window_right = 0;
VideoWindow* video_window_disp = 0;  

CvCapture* capture_left = 0;
CvCapture* capture_right = 0;

CvVideoWriter* writer_left = 0;
CvVideoWriter* writer_right = 0;
CvVideoWriter* writer_disp = 0;

static int started = 0;
double timeout = 0.005;
IplImage* video_image_left = 0;
IplImage* video_image_right = 0;
IplImage* video_image_disp = 0;
IplImage* disp_image = 0;
IplImage* lg = 0;
IplImage* rg = 0;

double old_pos = 0;
Fl_Value_Slider* video_pos = 0;
Fl_Check_Button* do_stereo_button = 0;
Fl_Check_Button* rectify_button = 0;
Fl_Check_Button* half_size_button = 0;

int guard = 0;
int is_avi = 0;
int is_recorded = 0;
double fps = -1;
double fps_alpha = 0.1;
char fps_buffer[100];
double cpu_freq = 0;
double prev_frame_stamp = 0;
int start_pos = 0;
int total_frames0 = 0;
int total_frames = 0;
int is_loopy = 0;

int cur_method = 0;

Fl_Button* stop_button = 0;
Fl_Button* play_button = 0;
Fl_Button* video_button = 0;
Fl_Button* cam_button = 0;
Fl_Button* record_button = 0;
Fl_Box* fps_box = 0;

Fl_Button* calib_button = 0;
Fl_Button* savecam_button = 0;
Fl_Button* loadcam_button = 0;
Fl_Button* swapcam_button = 0;

Fl_Value_Slider* max_disp_slider = 0;

Fl_Tabs* tabs = 0;

class VideoWindow : public Fl_Box
{
public:
    VideoWindow( int x, int y, int w, int h, const char* t = 0 );
    ~VideoWindow();
    void draw();
};

VideoWindow::VideoWindow( int x, int y, int w, int h, const char* t ):
    Fl_Box( x, y, w, h, t )
{
}

VideoWindow::~VideoWindow()
{
}

void VideoWindow::draw()
{
}

static void end_capture()
{
    started = 0;
    cvReleaseCapture( &capture_left );
    cvReleaseCapture( &capture_right );
}

static void cb_Stop( Fl_Widget*, void* );
static void cb_StartStopRecord( Fl_Widget*, void* );
static void cb_Exit( Fl_Widget*, void* );

static double get_time_accurate(void)
{
    return (double)cvGetTickCount()*1e-3/(cpu_freq+1e-10);
}

IplImage* stereo( IplImage* left, IplImage* right )
{
    if( lg && (left->width != lg->width || left->height != lg->height) )
    {
        cvReleaseImage(&lg); lg = NULL;
    }
    if( rg && (right->width != rg->width || right->height != rg->height) )
    {
        cvReleaseImage(&rg); rg = NULL;
    }
    if( disp_image && (left->width != disp_image->width || left->height != disp_image->height ))
    {
        cvReleaseImage(&disp_image); disp_image = NULL;
    }

    if( !disp_image )
        disp_image = cvCreateImage( cvSize(left->width, left->height),
                                    IPL_DEPTH_8U, 1 );
    if( !lg )
        lg = cvCreateImage( cvSize(left->width, left->height),
                                    IPL_DEPTH_8U, 1 );
    if( !rg )
        rg = cvCreateImage( cvSize(right->width, right->height),
                                    IPL_DEPTH_8U, 1 );

    switch( cur_method )
    {
    case 0: //Moodified Birchfield
        cvCvtColor( left,lg, CV_BGR2GRAY );
        cvCvtColor( right,rg, CV_BGR2GRAY );
        cvSet(disp_image, cvScalar(0,0,0,0) );

        if( half_size_button->value() )
        {
            IplImage* hl = cvCreateImage(cvSize(lg->width/2, lg->height/2),
                                     lg->depth, lg->nChannels );
            IplImage* hr = cvCreateImage(cvSize(rg->width/2, rg->height/2),
                                         rg->depth, rg->nChannels );
            IplImage* hd = cvCreateImage(cvSize(disp_image->width/2, disp_image->height/2),
                                         disp_image->depth, disp_image->nChannels );
        
            cvResize( lg,hl);
            cvResize( rg,hr);
            cvSet(hd, cvScalar(0,0,0,0) );
            
            cvFindStereoCorrespondence( 
                   hl, hr,
                   CV_DISPARITY_BIRCHFIELD,
                   hd,
                   max_disp ); 
            
            cvConvertScale( hd, hd, 255./max_disp );

            cvResize( hd, disp_image );

            cvReleaseImage(&hl);
            cvReleaseImage(&hr);
            cvReleaseImage(&hd);
        }
        else
        {
            cvFindStereoCorrespondence( 
                   lg, rg,
                   CV_DISPARITY_BIRCHFIELD,
                   disp_image,
                   max_disp );
        
            cvConvertScale(disp_image, disp_image, 255./max_disp);
        }

        break;
    default: 
        break;
    }  
    return disp_image;
}

void show_images()
{
    uchar* data = 0;
    int step = 0;
    CvSize size;
    cvGetRawData( video_image_left, &data, &step, &size );

    video_window_left->redraw();
    fl_draw_image( (uchar*)data, video_window_left->x(), video_window_left->y(),
                   size.width, size.height, 3, step );

    cvGetRawData( video_image_right, &data, &step, &size );

    video_window_right->redraw();
    fl_draw_image( (uchar*)data, video_window_right->x(), video_window_right->y(),
                   size.width, size.height, 3, step );  
}
       
static void get_next_frame(void*)
{
    static int repositioning = 0;
    IplImage* frame_left = 0;
    IplImage* frame_right = 0;

    double new_pos = video_pos->value();
    if( (new_pos-old_pos >= 1e-10 || new_pos-old_pos <= -1e-10) && !repositioning)
    {
        video_window_left->redraw();
        video_window_right->redraw();
        
        cvSetCaptureProperty( capture_left, CV_CAP_PROP_POS_AVI_RATIO, new_pos );
        cvSetCaptureProperty( capture_right, CV_CAP_PROP_POS_AVI_RATIO, new_pos );
        new_pos = cvGetCaptureProperty( capture_left, CV_CAP_PROP_POS_AVI_RATIO );
        printf("Repositioning\n");
        repositioning = 1;
    }
    else
    {
        new_pos = cvGetCaptureProperty( capture_left, CV_CAP_PROP_POS_AVI_RATIO );
        video_pos->value(new_pos);
        repositioning = 0;
    }
    old_pos = new_pos;
    frame_left = cvQueryFrame( capture_left );
    frame_right = cvQueryFrame( capture_right );

    if( (frame_left == 0 || frame_right == 0) && is_avi )
    {
        cb_Stop(0,0);
        return;
    }

    if( video_window_left && frame_left &&
        video_window_right && frame_right )
    {
        //resize video window if it is less than required
        if( video_window_left->w() < frame_left->width || video_window_left->h() < frame_left->height || 
            video_window_right->w() < frame_right->width || video_window_right->h() < frame_right->height  )
            root_window->size( (short)(frame_left->width + frame_right->width + 30), (short)(MAX(frame_left->height,frame_right->height) + 150));
        
        //draw left images
        CvRect rect = { video_window_left->x(), video_window_left->y(),
                        frame_left->width, frame_left->height };
        CvRect rect2 = { video_window_right->x(), video_window_right->y(),
                        frame_right->width, frame_right->height };
        
        if( !video_image_left || video_image_left->width < rect.width ||
            video_image_left->height < rect.height )
        {
            cvReleaseImage( &video_image_left );
            video_image_left = cvCreateImage( cvSize( rect.width, rect.height ), 8, 3 );
        }

        cvSetImageROI( video_image_left, cvRect(0,0,rect.width, rect.height));
        cvFlip( frame_left, video_image_left, 0 );
        
        if( writer_left && is_recorded )
        {
            cvWriteFrame( writer_left, video_image_left );
        }
        cvCvtColor( video_image_left, video_image_left, CV_RGB2BGR );
        
        if( !video_image_right || video_image_right->width < rect2.width ||
            video_image_right->height < rect2.height )
        {
            cvReleaseImage( &video_image_right );
            video_image_right = cvCreateImage( cvSize( rect2.width, rect2.height ), 8, 3 );
        }

        cvSetImageROI( video_image_right, cvRect(0,0,rect2.width, rect2.height));
        cvFlip( frame_right, video_image_right, 0 );

        if( writer_right && is_recorded )
        {
            cvWriteFrame( writer_right, video_image_right );
        } 
        cvCvtColor( video_image_right, video_image_right, CV_RGB2BGR );

        if( calib_button->value() )
        {
            IplImage* images[2] = { video_image_left, video_image_right };
            bool found = CalibFilter.FindEtalon( images );
            //if( !save )
                CalibFilter.DrawPoints( images );

            if( found )
            {
                int cur_time = clock();
                if( cur_time >= prev_time + 1000 )
                {
                    prev_time = cur_time;
                    CalibFilter.Push();
                    cvXorS( images[0], cvScalarAll(255), images[0] );
                    cvXorS( images[1], cvScalarAll(255), images[1] );
                    //g_undistort = 0;
                }

                if( CalibFilter.IsCalibrated() )
                {
                    calib_button->clear();
                    rectify_button->activate();
                    savecam_button->activate();

                    //g_undistort = 1;
                    //g_rectify = 1;
                }
            }
        }
        if( CalibFilter.IsCalibrated() && rectify_button->value() )
        {
            IplImage* images[2] = { video_image_left, video_image_right };
            CalibFilter.Rectify( images, images );
        }     

        //Running stereo algorithm
        //draw disparity window
        if( do_stereo_button->value() && video_window_disp  )
        {
            IplImage* disp; 
            disp = stereo(video_image_left, video_image_right);
        
            if( disp ) 
            {     
                CvRect rect = { video_window_disp->x(), video_window_disp->y(),
                            disp->width, disp->height };

                if( !video_image_disp || video_image_disp->width < rect.width ||
                    video_image_disp->height < rect.height )
                {
                    cvReleaseImage( &video_image_disp );
                    video_image_disp = cvCreateImage( cvSize( rect.width, rect.height ), 8, 3 );
                }

                if( disp->nChannels != 3 )
                {
                    cvCvtColor( disp, video_image_disp, CV_GRAY2BGR );
                }
                else
                {
                    cvCopy( disp, video_image_disp );
                }

                if( writer_disp && is_recorded )
                {
                    cvWriteFrame( writer_disp, video_image_disp );
                } 
        

                uchar* data = 0;
                int step = 0;
                CvSize size;
                cvGetRawData( video_image_disp, &data, &step, &size );

                video_window_disp->redraw();
                fl_draw_image( (uchar*)data, video_window_disp->x(), video_window_disp->y(),
                               size.width, size.height, 3, step );  
            }
        }   
        show_images();

    }           

    if( started )
    {
        double cur_frame_stamp = get_time_accurate();
        // update fps
        if( fps < 0 )
            fps = 1000/(cur_frame_stamp - prev_frame_stamp);
        else
            fps = (1-fps_alpha)*fps + fps_alpha*1000/(cur_frame_stamp - prev_frame_stamp);
        prev_frame_stamp = cur_frame_stamp;
        sprintf( fps_buffer, "FPS: %5.1f", fps );
        fps_box->label( fps_buffer );
        fps_box->redraw();
        if( total_frames > 0 )
        {
            if( --total_frames == 0 )
                if( !is_loopy )
                    cb_Exit(0,0);
                else
                {
                    total_frames = total_frames0;
                    cvSetCaptureProperty( capture_left, CV_CAP_PROP_POS_FRAMES, start_pos );
                    cvSetCaptureProperty( capture_right, CV_CAP_PROP_POS_FRAMES, start_pos );
                }
        }
        Fl::add_timeout( timeout, get_next_frame, 0 );
    }
}

static void cb_PauseResume( Fl_Widget*, void* )
{
    video_button->deactivate();
    cam_button->deactivate();
    stop_button->activate();

    if( !started )
    {
        started = 1;
        play_button->label("@||");
        play_button->activate();
        play_button->redraw();
        stop_button->activate();
        stop_button->redraw();
        record_button->activate();

        video_window_left->redraw();
        video_window_right->redraw();
        
        prev_frame_stamp = get_time_accurate();
        Fl::add_timeout( timeout, get_next_frame, 0 );
    }
    else
    {
        started = 0;
        play_button->label("@>");
        play_button->redraw();
    }
}


static void cb_Stop( Fl_Widget*, void* )
{
    if( started )
        cb_PauseResume(0,0);
    if( is_recorded )
        cb_StartStopRecord(0,0);
    cvSetCaptureProperty( capture_left, CV_CAP_PROP_POS_AVI_RATIO, 0 );
    cvSetCaptureProperty( capture_right, CV_CAP_PROP_POS_AVI_RATIO, 0 );
    video_button->activate();
    cam_button->activate();
    stop_button->deactivate();
    record_button->deactivate();
}

static void cb_StartAVI( const char* newfile_left, const char* newfile_right,
                         int start_pos, int was_started )
{
    if( newfile_left != 0 && newfile_right != 0)
    {
        end_capture();
        capture_left = cvCaptureFromAVI( newfile_left );
        capture_right = cvCaptureFromAVI( newfile_right );

        if( capture_left && capture_right )
        {
            was_started = 1;
            if( start_pos != 0 )
            {   
                cvSetCaptureProperty( capture_left, CV_CAP_PROP_POS_FRAMES, start_pos );
                cvSetCaptureProperty( capture_right, CV_CAP_PROP_POS_FRAMES, start_pos );
            }
        }
        is_avi = 1;
    }

    if( was_started )
        cb_PauseResume(0,0);
}

static void cb_LoadImages( const char* filename1, const char* filename2 )
{
    if( video_image_left )
        cvReleaseImage( &video_image_left );
    if( video_image_right )
        cvReleaseImage( &video_image_right );
    if( video_image_disp )
        cvReleaseImage( &video_image_disp );   

    video_image_left = cvLoadImage( filename1, 1 );
    video_image_right = cvLoadImage( filename2, 1 );
}

static void cb_Open( Fl_Widget*, void* )
{
    char filename1[1000];
    filename1[0] = '\0';
    char filename2[1000];
    filename2[0] = '\0';
    int was_started = 0;
    if( started )
    {
        was_started = 1;
        cb_PauseResume(0,0);
    }
    char *newfile = fl_file_chooser("Open Left File", "*", NULL);
    if( newfile ) 
    {
        strcpy( filename1, newfile );
        newfile = fl_file_chooser("Open Right File", "*", NULL);
        if( newfile )
        {
            strcpy( filename2, newfile );

            //TODO: add bitmap files processing
            char* point = strrchr( filename1, '.' );
            if( !strcmp( point, ".avi" ) )
                cb_StartAVI( filename1, filename2, 0, was_started );
            else if( !strcmp( point, ".bmp" ) )
                cb_LoadImages( filename1, filename2 );
        }
    }
}

void cb_LoadCamera( Fl_Widget*, void* )
{
    cb_PauseResume(0,0);

    char filename[1000];
    filename[0] = '\0';
    char *newfile = fl_file_chooser("Load Camera Parameters", "*", filename);

    if( newfile ) 
    {
        CalibFilter.LoadCameraParams( newfile );
        rectify_button->activate();
    }
    cb_PauseResume(0,0);
}

void cb_SaveCamera( Fl_Widget*, void* )
{
    cb_PauseResume(0,0);

    char filename[1000];
    filename[0] = '\0';
    char *newfile = fl_file_chooser("Save Camera Parameters", "*", filename);

    if( newfile ) CalibFilter.SaveCameraParams( newfile );

    cb_PauseResume(0,0);

}


void cb_StartCam( Fl_Widget*, void* )
{
    if( started )
        cb_PauseResume(0,0);
    end_capture();
#ifdef WIN32    
    Sleep(10);
#endif    
    is_avi = 0;
    capture_left = cvCaptureFromCAM( /*100*/-1 );
    capture_right = cvCaptureFromCAM( /*101*/-1 );
    cb_PauseResume(0,0);
}


static void cb_StartStopRecord( Fl_Widget*, void* )
{       
    if( !is_recorded )
    {
        cb_PauseResume(0,0);


        if( video_image_left )
        {
            char *newfile = fl_file_chooser("Left File", "*", NULL);
            writer_left = NULL;
            if( newfile ) 
            { 
                writer_left = cvCreateVideoWriter( newfile, -1, 15, cvGetSize( video_image_left ));
            }
            if( writer_left )
            {
                record_button->box( FL_DOWN_BOX );
                is_recorded = 1;
            }
        }
        if( video_image_right )
        {
            char *newfile = fl_file_chooser("Right File", "*", NULL);
            writer_right = cvCreateVideoWriter( newfile, -1, 15, cvGetSize( video_image_left ));
            if( writer_right )
            {
                record_button->box( FL_DOWN_BOX );
                is_recorded = 1;
            }
        }
        if( video_image_disp )
        {
            char *newfile = fl_file_chooser("Disparity File", "*", NULL);
            writer_disp = cvCreateVideoWriter( newfile, -1, 15, cvGetSize( video_image_disp ));
            if( writer_disp )
            {
                record_button->box( FL_DOWN_BOX );
                is_recorded = 1;
            }
        }  
        cb_PauseResume(0,0);
    }
    else
    {
        record_button->box( FL_UP_BOX );
        cvReleaseVideoWriter( &writer_left );
        cvReleaseVideoWriter( &writer_right );  
        cvReleaseVideoWriter( &writer_disp ); 

        is_recorded = 0;
    }
}

static void cb_AutoRun( void* _aviname )
{
    char* aviname_left = (char*)_aviname;
    char* aviname_right = (char*)_aviname;


   /* char* ext_pos = strrchr( aviname, '.' );
    if( ext_pos )
    {
        char* colon_pos = strchr( ext_pos, ':' );
        if( colon_pos )
        {
            *colon_pos = '\0';
            start_pos = strtoul( colon_pos+1, &colon_pos, 10 );
            if( colon_pos && *colon_pos == ':' )
                total_frames0 = total_frames = strtoul( colon_pos+1, &colon_pos, 10 );
            if( colon_pos && *colon_pos == ':' )
                is_loopy = 1;
        }
    }*/

    cb_StartAVI( aviname_left, aviname_right, start_pos, 0 );
}


static void cb_Exit( Fl_Widget*, void* )
{
    cvReleaseCapture( &capture_left );
    cvReleaseCapture( &capture_right );
    cvReleaseVideoWriter( &writer_left );
    cvReleaseVideoWriter( &writer_right );
    cvReleaseVideoWriter( &writer_disp ); 
    exit(0);
}


static void cb_OnSlider( Fl_Widget* slider, void* value)
{
    *((int*)value) = ((Fl_Slider*)slider)->value();

    //recompute stereo on current frame 
    if(!started && video_image_left && video_image_right)
    {
        IplImage* disp = stereo(video_image_left, video_image_right);
            
        if( disp ) 
        {     
            CvRect rect = { video_window_disp->x(), video_window_disp->y(),
                        disp->width, disp->height };
    
            if( !video_image_disp || video_image_disp->width < rect.width ||
                video_image_disp->height < rect.height )
            {
                cvReleaseImage( &video_image_disp );
                video_image_disp = cvCreateImage( cvSize( rect.width, rect.height ), 8, 3 );
            }

            if( disp->nChannels != 3 )
            {                   
                cvCvtColor( disp, video_image_disp, CV_GRAY2BGR );
            }
            else
            {
                cvCopy( disp, video_image_disp );
            }

            uchar* data = 0;
            int step = 0;
            CvSize size;
            cvGetRawData( video_image_disp, &data, &step, &size );

            video_window_disp->redraw();
            fl_draw_image( (uchar*)data, video_window_disp->x(), video_window_disp->y(),
                           size.width, size.height, 3, step ); 
            show_images();
        }
    }


}

static void cb_OnMethod( Fl_Widget* tab, void* )
{
     Fl_Group* v= (Fl_Group*)((Fl_Tabs*)tab)->value();
     //Compare tab label with method name
     const char* tab_name = v->label();

     cur_method = -1;
     for( int i = 0; methods_names[i]; i++)
     {
         if(!strcmp( tab_name, methods_names[i]) )
         {
             cur_method = i;
         }          
     }
}

static void cb_Calibrate( Fl_Widget* button, void* )
{
     Fl_Button* v= ((Fl_Button*)button);
     v->value(!v->value());
} 

static void cb_Swap( Fl_Widget* , void* )
{
    CvCapture* tmp = capture_left;
    capture_left = capture_right;
    capture_right = tmp;
}   

const int root_w = 320*3+30, 
          root_h = 240 /*image*/ + 100/*play controls*/+ 150/*method tabs*/ + 30;

int main( int argc, char **argv )
{
    char* aviname_left = 0;
    char* aviname_right = 0;
    int auto_run = 0;

    if( argc == 3 )
    {
        aviname_left = argv[1];
        aviname_right = argv[2];
        auto_run = 1;
    }
    cpu_freq = cvGetTickFrequency(); 
    printf("Processor frequency: %g\n", cpu_freq );

    CalibFilter.SetEtalon( CV_CALIB_ETALON_CHESSBOARD, EtalonParams );
    CalibFilter.SetCameraCount(2);
    CalibFilter.SetFrames(15);


    Fl_Window* w;
    {
        Fl_Window* o = root_window = new Fl_Window( root_w, root_h );
        w = o;
        {
            Fl_Tabs* o = new Fl_Tabs( 10, 10, root_w - 20, root_h - 250 );
            // camera tab
            {
                Fl_Group* o = new Fl_Group( 10, 30, root_w - 20, root_h - 250, "Stereo" );
                {
                    //left window
                    VideoWindow* o = new VideoWindow( 15, 35, (root_w - 30) / 3, root_h - 120 );
                    video_window_left = o;
                    o->box( FL_BORDER_BOX );
                    o->color(0);

                    //right window
                    o = new VideoWindow( 15 + (root_w - 30)/3, 35, (root_w - 30)/3, root_h - 120 );
                    video_window_right = o;
                    o->box( FL_BORDER_BOX );
                    o->color(0); 

                    //disparity window
                    o = new VideoWindow( 15 + (root_w - 30)/3*2, 35, (root_w - 30)/3, root_h - 120 );
                    video_window_disp = o;
                    o->box( FL_BORDER_BOX );
                    o->color(0); 
                }
                o->end();
            }
            o->end();
            Fl_Group::current()->resizable(o);
        }
        //tabs with different stereo methods
        {
            Fl_Tabs* o = new Fl_Tabs( 10, root_h - 230, root_w - 20, 150 );
            tabs = o;
            o->callback(cb_OnMethod);
            {
                for( int i = 0; methods_params[i]; i++ )
                {
                    Fl_Group* o = new Fl_Group( 10, root_h - 210, root_w - 20, 150, methods_names[i] );
                    {
                        //create controls for current method
                        for( int j = 0; methods_params[i][j].name; j++ )
                        {
                            switch( methods_params[i][j].type )
                            {
                            case TYPE_RANGE:{
                                Fl_Value_Slider* o = new Fl_Value_Slider( 15,root_h - 200 + j * 40, 200, 20, methods_params[i][j].name );
                                o->type( FL_HOR_NICE_SLIDER );
                                o->minimum(methods_params[i][j].minimum);
                                o->maximum(methods_params[i][j].maximum);
                                o->step(1);
                                o->callback(cb_OnSlider, methods_params[i][j].value );
                                *((int*)(methods_params[i][j].value)) = methods_params[i][j].minimum;

                                break;    }
                            case TYPE_BOOL:
                                break;
                            default: assert(0);
                            }
                        }                        
                    }
                    o->end();
                }
                {
                    //camera calibration tab
                    Fl_Group* o = new Fl_Group( 10, root_h - 210, root_w - 20, 150, "Camera" );
                    {
                        const int bwidth = 30, bheight = 30;
                        calib_button = new Fl_Button( 10 + bwidth, root_h - 200, bwidth, bheight, "C" );
                        calib_button->callback( cb_Calibrate );
                        savecam_button = new Fl_Button( 10 + bwidth*2, root_h - 200, 2*bwidth, bheight, "Save" );
                        loadcam_button = new Fl_Button( 10 + bwidth*4, root_h - 200, 2*bwidth, bheight, "Load" );
                        savecam_button->callback(cb_SaveCamera);
                        savecam_button->deactivate();
                        loadcam_button->callback(cb_LoadCamera);
                        swapcam_button = new Fl_Button( 10 + bwidth*6, root_h - 200, bwidth, bheight, "@<->" );
                        swapcam_button->callback(cb_Swap);
                    }
                    o->end();
                 }

            }
            o->end();                         
        }

        {
            const int bwidth = 30, bheight = 30;
            play_button = new Fl_Button( 10, root_h - 35, bwidth, bheight, "@>" );
            play_button->callback((Fl_Callback*)cb_PauseResume);
            play_button->deactivate();
            stop_button = new Fl_Button( 10 + bwidth, root_h - 35, bwidth, bheight, "@square" );
            stop_button->callback((Fl_Callback*)cb_Stop);
            stop_button->deactivate();
            video_button = new Fl_Button( 10 + bwidth*2, root_h - 35, bwidth, bheight, "..." );
            video_button->callback((Fl_Callback*)cb_Open);
            cam_button = new Fl_Button( 10 + bwidth*3, root_h - 35, bwidth, bheight, "[o]" );
            cam_button->callback((Fl_Callback*)cb_StartCam);
            video_pos = new Fl_Value_Slider( 10 + bwidth*4 + 10, root_h - 35, 200, 20, "Position" );
            video_pos->type( FL_HOR_NICE_SLIDER );
            record_button = new Fl_Button( 10 + bwidth*4 + 230, root_h - 35, bwidth, bheight, "@circle" );
            record_button->labelcolor(FL_RED);
            record_button->callback((Fl_Callback*)cb_StartStopRecord );
            record_button->deactivate();
            fps_box = new Fl_Box( 10, root_h - 75, bwidth*4, bheight, "<No data>" );
            fps_box->box( FL_DOWN_BOX );

            do_stereo_button = new Fl_Check_Button( 10 + bwidth*4 + 270, root_h - 35, 4*bwidth, bheight, "CalcStereo" );
            rectify_button = new Fl_Check_Button( 10 + bwidth*4 + 370, root_h - 35, 4*bwidth, bheight, "Rectify" );
            rectify_button->deactivate();
            half_size_button = new Fl_Check_Button( 10 + bwidth*4 + 470, root_h - 35, 4*bwidth, bheight, "Use half size" );
        }
        o->end();
    }
    Fl::visual(FL_RGB);
    w->show(argc, argv);
    if( auto_run )
        Fl::add_timeout( 0.1, cb_AutoRun, aviname_left );
    Fl::run();
    cb_Exit(0,0);
    return 0;
}
#endif
