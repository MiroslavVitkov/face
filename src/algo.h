#ifndef ALGO_H_
#define ALGO_H_


#include "except.h"

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>


namespace algo
{


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



// Tran a Local Bnary Patterns model.
struct TrainerLBP
{
    TrainerLBP( const std::string & fname_model );
    void update( int label, const cv::Mat & gray_face );
    void save() const;

private:
    cv::Ptr<cv::face::FaceRecognizer> _model;
    const std::string _fname_model;
    std::vector<int> _tmp_labels;
    std::vector<cv::Mat> _tmp_faces;
};


}  // namespace algo


#endif // defined(ALGO_H_)
