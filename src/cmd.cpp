#include "cmd.h"
#include "io.h"

#include <iostream>


namespace cmd
{

void PrintHelp::execute()
{
    std::cout << "Usage: aeolus [OPTIONS] [COMMAND FILE]\n"
                 "\n"
                 "This is a small image processing tool. "
                 "Currently, only PNG input and output files are supported.\n"
                 "\n"
                 "Commands:\n"
                 "view                - Show selected image file on screen, or copy it if -o is pecified.\n"
                 "region <X Y [DIST]> - Compute a same-colour region around coordinate.\n"
                 "                      DIST is (r1-r2)^2 + (g1-g2)^2 + (b1-b2)^2, default 0\n"
                 "\n"
                 "Option          GNU long option        Meaning\n"
                 "-h              --help                 Print this message and exit.\n"
                 "-o <filename>   --outfile <filename>   Write results to file; if omitted, result is shown on screen.\n";
}


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
            break;
        case Case::_dir_to_vid:
            break;
        case Case::_vid_to_dir:
            break;
    }
}








}  // namespace cmd
