#ifndef IFACE_H
#define IFACE_H

#include <opencv2/opencv.hpp>

#include <stdexcept>
#include <string>
#include <vector>


struct Exception : public std::runtime_error
{
    Exception( const std::string & msg )
    : std::runtime_error( msg ) { }
};


struct FrameStream
{
    virtual FrameStream & operator>>( cv::Mat & frame ) = 0;
    virtual ~FrameStream() = default;
};


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


struct Camera : public FrameStream
{
    enum class Id : int
    {
        // docs.opencv.org/3.2.0/d8/dfe/classcv_1_1VideoCapture.html#a5d5f5dacb77bbebdcbfb341e3d4355c1
        _first_usb_camera = 0
    };

    cv::VideoCapture _video_stream;

    Camera( Id );
    Camera & operator>>( cv::Mat & frame ) override;
};


struct VideoReader : public FrameStream
{
    cv::VideoCapture _video_stream;

    VideoReader( const std::string & path );
    VideoReader & operator>>( cv::Mat & frame ) override;
};


// Detect faces usng Local Bnary Patterns.
// https://docs.opencv.org/3.2.0/d1/de5/classcv_1_1CascadeClassifier.html
template < typename  >
struct LBP : public Detector
{
    FrameStream & _frame_stream;
    cv::CascadeClassifier _classifier;
    std::queue<cv::Mat> _detected_faces;
    std::vector<cv::Rect> _facerect_buff;

    LBP( const Settings & s );
    std::vector<cv::Mat> get_faces( const cv::Mat & frame
                                  , double min_confidence = 0.8
                                  ) override;
};



#endif // IFACE_H
