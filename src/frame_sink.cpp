#include "iface.h"

#include <dirent.h>


VideoWriter::VideoWriter( const std::string path
                        , cv::Size size
                        , Fit fit_mode )
    : _video_stream{ path, cv:: VideoWriter::fourcc('M','J','P','G'), 10, size }
    , _size{ size }
    , _fit{ fit_mode }
{
}


cv::Mat fit_frame( const cv::Mat & in, cv::Size target, VideoWriter::Fit mode )
{
    const auto iw = in.size().width;
    const auto ih = in.size().height;
    const auto tw = target.width;
    const auto th = target.height;

    assert( iw <= tw );
    assert( ih <= th );
    assert( iw < tw || ih < th );

    const auto top = (th - ih) / 2;
    const auto bottom = (th - ih) / 2 + (th - ih) % 2;
    const auto left = (tw - iw) / 2;
    const auto right = (tw - iw) / 2 + (tw - iw) % 2;

    cv::Mat out;  // BORDER_CONSTANT
    cv::copyMakeBorder( in, out, top, bottom, left, right, cv::BORDER_DEFAULT );

    assert( out.size().width == target.width );
    assert( out.size().height == target.height );

    return out;
}


VideoWriter & VideoWriter::operator<<( const cv::Mat & frame )
{
    assert( frame.data );
    assert( frame.size().width > 0 );
    assert( frame.size().height > 0 );

    if( frame.size().width == _size.width
     && frame.size().height == _size.height )
    {
        _video_stream << frame;
    }
    else
    {
        _video_stream << fit_frame( frame, _size, _fit );
    }


    return *this;
}


DirWriter::DirWriter( const std::string path )
{
    // todo: implement
    (void)path;
}


DirWriter & DirWriter::operator<<( const cv::Mat & frame )
{
    // todo: implement
    (void) frame;
    return *this;
}
