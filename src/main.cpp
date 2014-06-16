/*
 * main.cpp
 *
 *  Created on: Apr 28, 2013
 *      Author: Miroslav Vitkov
 */

#include <opencv2/opencv.hpp>

//#define DEMO_VARIANT_EDGE_DETECTION
#define DEMO_VARIANT_FACE_DETECTION

#ifdef DEMO_VARIANT_FACE_DETECTION
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

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
RNG rng(12345);
#endif  // DEMO_VARIANT_FACE_DETECTION

int main(int argc, char *argv[]){
    cv::VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())      // check if we succeeded
        return -1;

#ifdef DEMO_VARIANT_EDGE_DETECTION
    cv::Mat edges;
    cv::namedWindow("edges",1);
    for(;;)
    {
    	cv::Mat frame;
        cap >> frame; // get a new frame from camera
//        cv::imshow("edges", frame);

        cv::cvtColor(frame, edges, CV_BGR2GRAY);
        cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
        cv::Canny(edges, edges, 0, 30, 3);
        cv::imshow("edges", edges);
        if(cv::waitKey(30) >= 0) break;
    }
#endif  // DEMO_VARIANT_EDGE_DETECTION

#ifdef DEMO_VARIANT_FACE_DETECTION
   Mat frame;

   //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading_1\n"); return -1; };
   if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading_2\n"); return -1; };

   //-- 2. Read the video stream
    while( true )
    {
		cap >> frame;

		//-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       { detectAndDisplay( frame ); }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
    }
#endif  // DEMO_VARIANT_FACE_DETECTION
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}


#ifdef DEMO_VARIANT_FACE_DETECTION
/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( size_t i = 0; i < faces.size(); i++ )
  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

    Mat faceROI = frame_gray( faces[i] );
    std::vector<Rect> eyes;

    //-- In each face, detect eyes
    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( size_t j = 0; j < eyes.size(); j++ )
     {
       Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
       circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
     }
  }
  //-- Show what you got
  imshow( window_name, frame );
}
 #endif  // DEMO_VARIANT_FACE_DETECTION