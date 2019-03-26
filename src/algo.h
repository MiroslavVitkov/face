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
struct DetectorLBP : public Detector
{
    DetectorLBP( const std::string & cascades_dir );
    std::vector<cv::Rect> get_face_rects( const cv::Mat & frame
                                        , double min_confidence = 0.8
                                        );
    std::vector<cv::Mat> get_faces( const cv::Mat & frame
                                  , double min_confidence = 0.8
                                  ) override;

private:
    cv::CascadeClassifier _classifier;
};



// Detect faces usng Local Bnary Patterns.
// https://docs.opencv.org/3.2.0/d1/de5/classcv_1_1CascadeClassifier.html
struct LBPRecogniser : public Recogniser
{
    using Label = std::string;

    LBPRecogniser( const std::string & cascades_dir );

private:
    cv::CascadeClassifier _classifier;
};


#endif // defined(ALGO_H_)
