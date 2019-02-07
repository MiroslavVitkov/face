// Watches through the main video camera.
// When a human face is detected, it is croppd
// and output on stdout.
//
// depends: libopencv-dev


#include <opencv2/opencv.hpp>

#include <iostream>
#include <sstream>


void detectAndDisplay( cv::Mat frame );


 /** Global variables */
// /usr/share/opencv/haarcascades
std::string face_cascade_name{ "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml"};
std::string eyes_cascade_name{ "/usr/share/opencv/haarcascades/haarcascade_eye_tree_eyeglasses.xml"};
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eyes_cascade;
std::string window_name = "Capture - Face detection";
const std::string pictures_dir = "./pictures";
cv::RNG rng(12345);


//
//  main()
//
int main( int argc, char *argv[] )
{
    cv::VideoCapture cap( 0 );
    if( ! cap.isOpened() )
    {
        std::cerr << "Failed to open default camera.";
        return -1;
    }

   cv::Mat frame;

   //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading_1\n"); return -1; };
   if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading_2\n"); return -1; };

   //-- 2. Read the video stream
    while( true )
    {
	cap >> frame;

	//-- 3. Apply the algorithm to the frame
       if( !frame.empty() )
       { detectAndDisplay( frame ); }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = cv::waitKey(10);
       if( (char)c == 'c' ) { break; }
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( cv::Mat frame )
{
  std::vector<cv::Rect> faces;
  cv::Mat frame_gray;

  cv::cvtColor( frame, frame_gray, CV_BGR2GRAY );
  cv::equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );

  // If there are any faces in the picture, count to 3. If this is the third in a row,
  // record the picture. This protects from false positives.
  static unsigned consecutive_frames_with_faces = 0;
  if(faces.empty())
  {
    consecutive_frames_with_faces = 0;
  } else
  {
    consecutive_frames_with_faces++;
    std::cout << faces.size() << " faces in image, image_count is " << consecutive_frames_with_faces << std::endl;

    static unsigned picture_number = 0;
    if(consecutive_frames_with_faces >= 3)
    {
      // Form file name.
      std::stringstream ss;
      ss << pictures_dir << "/face_" << picture_number++ << ".jpg";
      std::cout << "Writing picture " << ss.str() << std::endl;

      // Crop the faces from the picture and write them to a file.
      for(size_t i = 0; i < faces.size(); ++i)
      {
        cv::Rect myROI(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
        cv::Mat cropped = frame(myROI);
        cv::imwrite(ss.str(), cropped);
      }  // for each face in image
    }  // if lpf
  }  // if any faces in image
}  // foo()
