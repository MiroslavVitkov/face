// Detect faces in a video file or a usb camera.
// Output cropped faces on stdout or in an image folder.
//
// build-deps: libopencv-dev
// run-deps: haarcascade XMLs


#include "iface.h"

#include <queue>
#include <iostream>
#include <sstream>


cv::CascadeClassifier create_classifier( const std::string & cascades_dir
                                       , const std::string & name )
{
    const auto xml_path = cascades_dir + '/' + name + ".xml";
    cv::CascadeClassifier c( xml_path );
    if( c.empty() )
    {
        Exception e{ "Failed to load cascade classifier: " + xml_path };
    }

    return c;
}


LBP::LBP( const std::string & cascades_dir )
:_classifier{ create_classifier( cascades_dir, "haarcascade_frontalface_alt" ) }
{
    // todo: consider turning '_classifier' into a verctor
    // and adding at least 'haarcascade_eye_tree_eyeglasses'.
}


std::vector<cv::Mat> LBP::get_faces( const cv::Mat & frame
                                   , double min_confidence
                                   )
{
    assert( ! frame.empty() );

    // Convert to 8-bit single channel image.
    cv::Mat gray;
    cv::cvtColor( frame, gray, CV_BGR2GRAY );

    // Increase contrast in under- or over-exposed areas of the image.
    cv::equalizeHist( gray, gray );

    std::vector<cv::Rect> rects;
    _classifier.detectMultiScale( gray, rects );
    // Perhaps do low-pass filtering over 3 consecutive frames to evade false positives.
    (void)min_confidence;

    // Crop faces out of the video frame.
    std::vector<cv::Mat> faces;
    for( const auto & r : rects )
    {
        const auto cropped = frame( r );
        faces.push_back( cropped );
    }

    return faces;
}
