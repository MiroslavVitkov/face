#ifndef IFACE_H
#define IFACE_H

#include <opencv2/opencv.hpp>

#include <stdexcept>
#include <string>
#include <vector>


struct Exception : public std::runtime_error
{
    Exception( const std::string & msg )
    : std::runtime_error( msg )
    {
    }
};


struct FrameSource
{
    virtual cv::Mat operator>>( FrameSource & ) = 0;
    virtual ~FrameSource() = default;
};


struct FrameSink
{
    virtual void operator<<( const cv::Mat & ) = 0;
    virtual ~FrameSink() = default;
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
};



#endif // IFACE_H
