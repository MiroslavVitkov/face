#include "iface.h"

#include <dirent.h>

#include <regex>


Camera::Camera( Camera::Id id )
    : _video_stream{ static_cast< int >( id ) }
{
    if( ! _video_stream.isOpened() )
    {
        Exception e{ "Failed to initialize camera: "
                   + std::to_string( static_cast< int >( id ) ) };
        throw e;
    }
}


Camera & Camera::operator>>( cv::Mat & frame )
{
    _video_stream >> frame;
    return *this;
}


VideoReader::VideoReader( const std::string & path )
    : _video_stream{ path }
    , _good{ true }
{
    if( ! _video_stream.isOpened() )
    {
        Exception e{ "Failed to open video file: " + path };
        throw e;
    }
}


VideoReader::operator bool() const
{
    return _good;
}


VideoReader & VideoReader::operator>>( cv::Mat & frame )
{
    const auto b = _video_stream.read( frame );

    if( ! b )
    {
        assert( ! frame.data );
        _good = false;
    }
    else
    {
        assert( frame.data );
    }

    return *this;
}


std::string get_last_dir( std::string path )
{
    // Trailing foreward slash.
    while( path.back() == '/' )
    {
        path.pop_back();
    }

    // Relative path.
    const auto pos = path.rfind( '/' );
    if( pos == std::string::npos )
    {
        return path;
    }
    else
    {
        return path.substr( pos + 1 );
    }
}


// True if entry is the unix current of parent directory.
bool is_auto_dir( const struct dirent * const ent )
{
    bool is_currdir = ! std::string( ent->d_name ).compare( "." );
    bool is_pardir  = ! std::string( ent->d_name ).compare( ".." );
    return is_currdir || is_pardir;
}


struct DirReader::Impl
{
    Impl( const std::string & path )
        : _path{ path }
        , _label{ get_last_dir( path ) }
        , _dir{ opendir( path.c_str() ) }
        , _stream{ readdir( _dir ) }
        , _good{ true }
    {
        if( ! _dir || ! _stream )
        {
            Exception e{ "Failed to open faces directory: " + path };
            throw e;
        }
    }


    operator bool() const
    {
        return _good;
    }


    Impl & operator>>( cv::Mat & face )
    {
        assert( _dir );
        _stream = readdir( _dir );
        assert( _stream );

        if( is_auto_dir( _stream ) )
        {
            return ( *this >> face );
        }

        std::string file = _path + '/' + _stream->d_name;
        auto frame = cv::imread( file, CV_LOAD_IMAGE_GRAYSCALE );
        if( ! frame.data )
        {
            Exception e{ "Failed to read face file: " + file };
            throw e;
        }

        return *this;
    }


    const std::string & get_label() const
    {
        return _label;
    }


    ~Impl()
    {
        closedir( _dir );
    }


private:
    const std::string _path;
    const std::string _label;
    DIR * _dir;
    dirent * _stream;
    bool _good;
};


DirReader::DirReader( const std::string & path )
    : _impl{ std::make_unique<Impl>( path ) }
{
}


DirReader::DirReader( DirReader && other )
    : _impl{ std::move( other._impl ) }
{
}


DirReader::~DirReader()
{
}


DirReader::operator bool() const
{
    return ( !! *_impl );
}


DirReader & DirReader::operator>>( cv::Mat & face )
{
    *_impl >> face;
    return *this;
}


std::string DirReader::get_label() const
{
    return _impl->get_label();
}


std::vector<DirReader> get_subdirs( const std::string dataset_path )
{
    DIR * dir = opendir( dataset_path.c_str() );
    if( ! dir )
    {
        Exception e{ "Failed to open dataset dir: " + dataset_path };
        throw e;
    }

    std::vector<DirReader> out;
    while( struct dirent * stream = readdir( dir ) )
    {
        assert( stream );

        if( is_auto_dir( stream ) )
        {
            continue;
        }

        std::string dirpath = dataset_path + '/' + stream->d_name;
        try
        {
            out.emplace_back( DirReader{ dirpath } );
        }
        catch( ... )
        {
            closedir( dir );
            throw;
        }
    }
    closedir( dir );

    return out;
}
