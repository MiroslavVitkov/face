#include "cmd.h"
#include "io.h"

#include <iostream>


namespace cmd
{


void cam_to_vid( unsigned frames, std::string fname_out )
{
    fname_out += ".avi";

    Camera c;
    VideoWriter vw{ fname_out, c.get_size() };
    cv::Mat f;

    for( unsigned i = 0; i < frames; ++i )
    {
        c >> f;
        vw << f;
    }

    std::cout << "Wrote video file " << fname_out << " from default camera.\n";
}



void vid_to_vid( unsigned frames
               , const std::string & fname_in
               , const std::string & fname_out )
{
    VideoReader vr( fname_in );
    VideoWriter vw( fname_out, vr.get_size() );
    cv::Mat f;

    unsigned counter{};
    while( vr >> f && ++counter <= frames )
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


void vid_to_dir( unsigned frames
               , const std::string & in
               , const std::string & out )
{
    cv::Mat m;
    VideoReader v{ in };
    DirWriter w{ out };

    unsigned counter{};
    while( v >> m && ++counter <= frames )
    {
        w << m;
    }
}


Test::Test( Case c
          , unsigned frames
          , const std::string & source_path
          , const std::string & dest_path)
    : _case{ c }
    , _frames{ frames }
    , _source_path{ source_path }
    , _dest_path{ dest_path }
{
}



void Test::execute()
{
    switch( _case )
    {
        case Case::_cam_to_vid:
            cam_to_vid( _frames, _dest_path );
            break;
        case Case::_vid_to_vid:
            vid_to_vid( _frames, _source_path, _dest_path );
            break;
        case Case::_dir_to_vid:
            dir_to_vid( _source_path );
            break;
        case Case::_vid_to_dir:
            vid_to_dir( _frames, _source_path, _dest_path );
            break;
    }
}


}  // namespace cmd
