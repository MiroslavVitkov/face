#include "iface.h"

#include <dirent.h>


VideoWriter::VideoWriter( cv::Size size, const std::string path )
    : _video_stream{ path, cv:: VideoWriter::fourcc('M','J','P','G'), 10, size }
{
}


VideoWriter & VideoWriter::operator<<( const cv::Mat & frame )
{
    _video_stream << frame;
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
