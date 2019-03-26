#include "io.h"

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <regex>


namespace io
{


Camera::Camera( Mode mode, Camera::Id id )
    : _video_stream{ static_cast< int >( id ) }
    , _mode{ mode }
{
    if( ! _video_stream.isOpened() )
    {
        throw Exception{ "Failed to initialize camera: "
                         + std::to_string( static_cast< int >( id ) ) };
    }

    assert( _video_stream.get( cv::CAP_PROP_FPS ) - 30 < 0.000001 );
}


cv::Size get_stream_size( const cv::VideoCapture & stream )
{
    const auto w = stream.get( cv::CAP_PROP_FRAME_WIDTH );
    const auto h = stream.get( cv::CAP_PROP_FRAME_HEIGHT );
    const auto ww = static_cast<cv::Size::value_type>( round( w ) );
    const auto hh = static_cast<cv::Size::value_type>( round( h ) );
    return { ww, hh };
}


cv::Size Camera::get_size() const
{
    return get_stream_size( _video_stream );
}


Camera & Camera::operator>>( cv::Mat & frame )
{
    switch ( _mode )
    {
        case Mode::_colour:
            _video_stream >> frame;
            break;
        case Mode::_grayscale:
            cv::Mat tmp;
            _video_stream >> tmp;
            cv::cvtColor( tmp, frame, CV_BGR2GRAY );
            break;
    }

    return *this;
}


VideoReader::VideoReader( const std::string & path )
    : _video_stream{ path }
    , _good{ true }
{
    if( ! _video_stream.isOpened() )
    {
        throw Exception{ "Failed to open video file: " + path };
    }
}


VideoReader::operator bool() const
{
    return _good;
}


cv::Size VideoReader::get_size() const
{
    return get_stream_size( _video_stream );
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


cv::Size calc_largest_size( const std::string & path )
{
    cv::Size ret{};

    DirReader r{ path, Mode::_grayscale, false };
    cv::Mat f;
    while( r >> f )
    {
        if( f.size().width > ret.width )
        {
            ret = cv::Size{ f.size().width, ret.height };
        }
        if( f.size().height > ret.height )
        {
            ret = cv::Size{ ret.width, f.size().height };
        }
    }

    return ret;
}


struct DirReader::Impl
{
    Impl( const std::string & path, Mode mode, bool calc_size )
        : _path{ path }
        , _label{ get_last_dir( path ) }
        , _read_mode{ mode }
        , _dir{ opendir( path.c_str() ) }
        , _stream{ nullptr }
        , _size{}
    {
        if( ! _dir )
        {
            throw Exception{ "Failed to open faces directory: " + path };
        }

        // Expensive!
        if( calc_size )
        {
             _size = calc_largest_size( _path );
        }
    }


    operator bool() const
    {
        return !! _stream;
    }


    cv::Size get_size() const
    {
        assert( _size != cv::Size{} );
        return _size;
    }


    Impl & operator>>( cv::Mat & face )
    {
        assert( _dir );

        _stream = readdir( _dir );
        if( ! _stream )
        {
            return *this;
        }

        if( is_auto_dir( _stream ) )
        {
            return ( *this >> face );
        }

        const auto flags = [ this ] ()
        {
            switch ( _read_mode )
            {
                case Mode::_colour:
                    return CV_LOAD_IMAGE_UNCHANGED;
                case Mode::_grayscale:
                    return CV_LOAD_IMAGE_GRAYSCALE;
            }
            assert( false );
        } ();

        const auto file = _path + '/' + _stream->d_name;

        face = cv::imread( file, flags );
        if( ! face.data )
        {
            throw Exception{ "Failed to read face file: " + file };
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
    const Mode _read_mode;
    DIR * _dir;
    dirent * _stream;
    cv::Size _size;  // largest width and height
};


DirReader::DirReader( const std::string & path, Mode mode, bool calc_size )
    : _impl{ std::make_unique<Impl>( path, mode, calc_size ) }
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


cv::Size DirReader::get_size() const
{
    return _impl->get_size();
}


DirReader & DirReader::operator>>( cv::Mat & face )
{
    *_impl >> face;
    return *this;
}


const std::string & DirReader::get_label() const
{
    return _impl->get_label();
}


std::vector<DirReader> get_subdirs( const std::string & dataset_path
                                  , Mode mode
                                  , bool calc_size )
{
    DIR * dir = opendir( dataset_path.c_str() );
    if( ! dir )
    {
        throw Exception{ "Failed to open dataset dir: " + dataset_path };
    }

    std::vector<DirReader> out;
    while( struct dirent * stream = readdir( dir ) )
    {
        assert( stream );

        if( is_auto_dir( stream ) )
        {
            continue;
        }

        const auto dirpath = dataset_path + '/' + stream->d_name;
        try
        {
            out.emplace_back( DirReader{ dirpath, mode, calc_size } );
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


void draw_rects( cv::Mat & frame, const std::vector<cv::Rect> & rects )
{
    for( const auto & r : rects )
    {
        cv::rectangle( frame, r, cv::Scalar(0, 255, 0) );
    }
}


cv::Mat crop( const cv::Mat & frame, const cv::Rect & rect )
{
    return frame( rect );
}


std::vector<cv::Mat> crop( const cv::Mat & frame
                         , const std::vector<cv::Rect> & rects )
{
    std::vector<cv::Mat> ret;
    for( const auto & r : rects )
    {
        const auto cropped = frame( r );
        ret.emplace_back( cropped );
    }

    return ret;
}


VideoPlayer::VideoPlayer(const std::string & window_name )
    : _window_name{ window_name }
{
    cv::namedWindow( window_name, cv::WINDOW_AUTOSIZE );
}


VideoPlayer & VideoPlayer::operator<<( const cv::Mat & frame )
{
    cv::imshow( _window_name, frame );
    cv::waitKey( 30 );

    return *this;
}


VideoWriter::VideoWriter( const std::string & path, cv::Size size, Fit fit_mode )
    : _video_stream{ path, cv:: VideoWriter::fourcc('M','J','P','G'), 30, size }
    , _size{ size }
    , _fit{ fit_mode }
{
}


cv::Mat fit_frame( const cv::Mat & in, cv::Size target, VideoWriter::Fit )
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

    cv::Mat out;
    cv::copyMakeBorder( in, out, top, bottom, left, right, cv::BORDER_CONSTANT );

    assert( out.size().width == target.width );
    assert( out.size().height == target.height );

    return out;
}


VideoWriter & VideoWriter::operator<<( const cv::Mat & frame )
{
    assert( frame.data );
    assert( frame.size().width > 0 );
    assert( frame.size().height > 0 );

    if( frame.size() == _size )
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
    : _path{ path }
    , _frame_num{}
{
    if( mkdir( path.c_str(), 0777) )
    {
        throw Exception{ "Failed to create directory " + path };
    }
}


DirWriter & DirWriter::operator<<( const cv::Mat & frame )
{
    const auto name = _path + '/' + std::to_string( _frame_num ) + ".jpg";
    cv::imwrite( name, frame );

    ++_frame_num;
    assert( _frame_num != 0 );

    return *this;
}


}  // namespace io
