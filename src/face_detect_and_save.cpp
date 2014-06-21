/*
 * main.cpp
 *
 *  Created on: Apr 28, 2013
 *      Author: Miroslav Vitkov
 */

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <sstream>

using namespace std;
using namespace cv;

 /** Function Headers */
void detectAndDisplay( Mat frame );

 /** Global variables */
String face_cascade_name = "haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
const string pictures_dir = "./pictures";
RNG rng(12345);


//
//  main()
//
int main(int argc, char *argv[]){
    cv::VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())      // check if we succeeded
        return -1;

   Mat frame;

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

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  // If there are any faces in the picture, count to 3. If this is the third in a row,
  // record the picture. This protects from false positives.
  static unsigned consecutive_frames_with_faces = 0;
  if(faces.empty())
  {
    consecutive_frames_with_faces = 0;
  } else
  {
    consecutive_frames_with_faces++;
    cout << faces.size() << " faces in image, image_count is " << consecutive_frames_with_faces << endl;

    static unsigned picture_number = 0;
    if(consecutive_frames_with_faces >= 3)
    {
      // Form file name.
      stringstream ss;
      ss << pictures_dir << "/face_" << picture_number++ << ".jpg";
      cout << "Writing picture " << ss.str() << endl;

      // Crop the faces from the picture and write them to a file.
      for(size_t i = 0; i < faces.size(); ++i)
      {
        Rect myROI(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
        Mat cropped = frame(myROI);
        imwrite(ss.str(), cropped);
      }  // for each face in image
    }  // if lpf
  }  // if any faces in image
}  // foo()