#ifndef ALGO_H_
#define ALGO_H_


#include "except.h"

#include <opencv2/opencv.hpp>


struct Detector
{
    virtual std::vector<cv::Mat> get_faces( const cv::Mat & frame
                                          , double min_confidence = 0.8
                                          ) = 0;
    virtual ~Detector() = default;
};


struct Recogniser
{
    virtual std::string recognise( const cv::Mat & face
                                 , double min_confidence = 0.8
                                 ) = 0;
    virtual ~Recogniser() = default;
};


// Detect faces usng Local Bnary Patterns.
// https://docs.opencv.org/3.2.0/d1/de5/classcv_1_1CascadeClassifier.html
struct LBPDetector : public Detector
{
    LBPDetector( const std::string & cascades_dir );
    std::vector<cv::Rect> get_face_rects( const cv::Mat & frame
                                        , double min_confidence = 0.8
                                        );
    std::vector<cv::Mat> get_faces( const cv::Mat & frame
                                  , double min_confidence = 0.8
                                  ) override;
};


#endif // defined(ALGO_H_)
