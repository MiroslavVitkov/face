#include "iface.h"

#include <dirent.h>

#include <string>


// Save cropped faces in subdirectories.
// Each subdiretory is named after the label and contains positive images.
struct DirSink : public FrameSink
{
    struct Images
    {
        using Id = int;

        // Input data to the recogniser training algorithm.
        // Needs to be exactly in this format.
        std::vector<cv::Mat> _images;
        std::vector<Id> _labels;
        std::map<Id, std::string> _id_to_name;
    };


    cv::VideoCapture _video_stream;


    enum class Id
    {
        // docs.opencv.org/3.2.0/d8/dfe/classcv_1_1VideoCapture.html#a5d5f5dacb77bbebdcbfb341e3d4355c1
        _first_usb_camera = 0
    };


    DirSink( Id id )
    : _video_stream{ static_cast<int>( id ) }
    {
    }


    DirSink & operator>>( cv::Mat & frame )
    {
        _video_stream >> frame;
        return *this;
    }


private:
    // True if entry is the unix current of parent directory.
    static bool is_auto_dir( struct dirent *ent )
    {
        bool is_currdir = ! std::string( ent->d_name ).compare( "." );
        bool is_pardir  = ! std::string( ent->d_name ).compare( ".." );
        if( is_currdir || is_pardir )  return true;
        else                           return false;
    }


    // Images are expected to be distributed among sub-dirs inside a single dir.
    // One subdir per person.
    Images read_images( const std::string & path )
    {
        // Go to selected path.
        DIR *parent_dir = opendir( path.c_str() );
        if( parent_dir == nullptr)
        {
            std::cerr << "Could not open target location.";
            std::cerr << "Make sure path to a directory was supplied as argv[1]." << std::endl;
            std::exit(-1);
        }

        // Iterate each subdir.
        Images::Id i = 0;
        Images out;
        while( struct dirent * d1 = readdir(parent_dir) )
        {
            if( is_auto_dir(d1) )  continue;

            DIR *dir = opendir( (path + d1->d_name).c_str() );
            if( dir == nullptr )
            {
                std::cerr << "Failed opening subdirectory " << d1->d_name << std::endl;
                continue;
            }

            // Read all images inside the dir.
            while( struct dirent * d2 = readdir( dir ) )
            {
                if( is_auto_dir(d2) )  continue;
                std::string file = path + d1->d_name + '/' + d2->d_name;
                out._images.emplace_back( cv::imread( file, CV_LOAD_IMAGE_GRAYSCALE ) );
                out._labels.emplace_back( i );
            }
            closedir( dir );

            // Record the name of the dir, map a numeric counter to it.
            out._id_to_name.emplace( i, d1->d_name );
            ++i;
        }
        closedir( parent_dir );

        return out;
    }
};


struct VideoSink : public FrameSource
{
    cv::VideoCapture _video_stream;


    VideoIn( const std::string & path )
    : _video_stream{ path }
    {
    }


    VideoIn & operator>>( cv::Mat & frame )
    {
        _video_stream >> frame;
        return *this;
    }
};
