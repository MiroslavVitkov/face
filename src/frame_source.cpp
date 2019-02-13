#include "iface.h"


Camera::Camera( Camera::Id id )
: _video_stream{ static_cast< int >( id ) }
{
}


Camera & Camera::operator>>( cv::Mat & frame )
{
    _video_stream >> frame;
    return *this;
}


VideoReader::VideoReader( const std::string & path )
: _video_stream{ path }
{
}


VideoReader & VideoReader::operator>>( cv::Mat & frame )
{
    _video_stream >> frame;
    return *this;
}

