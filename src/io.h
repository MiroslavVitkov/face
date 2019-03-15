#ifndef IO_H_
#define IO_H_


#include "except.h"

#include <opencv2/opencv.hpp>

#include <memory>
#include <string>
#include <vector>


struct FrameSource
{
    virtual FrameSource & operator>>( cv::Mat & frame ) = 0;
    virtual operator bool() const = 0;  // true if last operation was successful
    virtual cv::Size get_size() const = 0;
    virtual ~FrameSource() = default;
};


struct FrameSink
{
    virtual FrameSink & operator<<( const cv::Mat & frame ) = 0;
    virtual ~FrameSink() = default;
};


struct Camera : public FrameSource
{
    enum class Id : int
    {
        // docs.opencv.org/3.2.0/d8/dfe/classcv_1_1VideoCapture.html#a5d5f5dacb77bbebdcbfb341e3d4355c1
        _first_usb_camera = 0
    };

    Camera( Id = Id::_first_usb_camera );
    operator bool() const override { return true; }
    cv::Size get_size() const override;
    Camera & operator>>( cv::Mat & frame ) override;

private:
    cv::VideoCapture _video_stream;
};


struct VideoReader : public FrameSource
{
    VideoReader( const std::string & path );
    operator bool() const override;
    cv::Size get_size() const override;
    VideoReader & operator>>( cv::Mat & frame ) override;

private:
    cv::VideoCapture _video_stream;
    bool _good;
};


// A directory with one subdirectory per subject.
// Subdirectory names are the labels.
// Each subdirectory contains cropped faces of that one subject.
struct DirReader : public FrameSource
{
    enum class ReadMode
    {
        _unchanged,
        _grayscale,
    };

    DirReader( const std::string & path
             , bool calc_size = false
             , ReadMode rm = ReadMode::_unchanged );
    DirReader( DirReader && );
    ~DirReader() override;

    operator bool() const override;
    DirReader & operator>>( cv::Mat & face ) override;
    cv::Size get_size() const override;
    std::string get_label() const;

private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
};


std::vector<DirReader> get_subdirs( const std::string dataset_path
                                  , bool calc_size = false );


struct VideoPlayer : FrameSink
{
    VideoPlayer( const std::string & window_name );
    FrameSink & operator<<( const cv::Mat & frame ) override;

private:
    const std::string _window_name;
};


struct VideoWriter : public FrameSink
{
    // How to fit smaller frames to the video.
    enum class Fit{ _border };

    VideoWriter( const std::string path
               , cv::Size
               , Fit fit_mode = Fit::_border );
    VideoWriter & operator<<( const cv::Mat & frame ) override;

private:
    cv::VideoWriter _video_stream;
    const cv::Size _size;
    const Fit _fit;
};


struct DirWriter : public FrameSink
{
    DirWriter( const std::string path );
    DirWriter & operator<<( const cv::Mat & frame ) override;

private:
    const std::string _path;
    long unsigned _frame_num;
};


#endif // defined(IO_H_)
