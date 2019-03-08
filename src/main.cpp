#include "iface.h"

#include <iostream>


void cam_to_vid( const std::string fname_out = "cam.avi" )
{
    Camera c;
    VideoWriter vw{ fname_out, c.get_size() };
    cv::Mat f;
    for( unsigned i = 0; i < 100; ++i )
    {
        c >> f;
        vw << f;
    }

    std::cout << "Wrote video file " << fname_out << " from default camera.\n";
}


void vid_to_vid( const std::string & fname_in = "cam.avi"
               , const std::string & fname_out = "vid.avi" )
{
    VideoReader vr( fname_in );
    VideoWriter vw( fname_out, vr.get_size() );
    cv::Mat f;

    while( vr >> f )
    {
        vw << f;
    }

    std::cout << "Copied video " << fname_in
              << " frame by frame to " << fname_out + '\n';
}


void dir_to_vid( const std::string & path = "./dataset" )
{
    auto cc = get_subdirs( path, true );
    cv::Mat m;
    std::cout << "Found the following subdirs:\n"
                 "(writing one video file for each)\n";
    for( auto & stream : cc )
    {
        VideoWriter vw{ stream.get_label() + ".avi", stream.get_size() };
        std::cout << stream.get_label() << std::endl;

        while( stream >> m )
        {
            vw << m;
        }
    }
}


void vid_to_dir( const std::string & in = "vid.avi"
               , const std::string & out = "frames" )
{
    cv::Mat m;
    VideoReader v{ in };
    DirWriter w{ out };

    while( v >> m )
    {
        w << m;
    }
}


int main()
{
    cam_to_vid();
    vid_to_vid();
    dir_to_vid();
    vid_to_dir();
}
