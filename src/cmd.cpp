#include "cmd.h"

#include "algo.h"
#include "io.h"

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

#include <iostream>


namespace cmd
{


void cam_to_vid( unsigned frames, std::string fname_out )
{
    fname_out += ".avi";

    io::Camera c;
    io::VideoWriter vw{ fname_out, c.get_size() };
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
    io::VideoReader vr( fname_in );
    io::VideoWriter vw( fname_out, vr.get_size() );
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
    auto cc = io::get_subdirs( path, io::Mode::_grayscale, true );
    cv::Mat m;
    std::cout << "Found the following subdirs:\n"
                 "(writing one video file for each)\n";
    for( auto & stream : cc )
    {
        io::VideoWriter vw{ stream.get_label() + ".avi", stream.get_size() };
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
    io::VideoReader v{ in };
    io::DirWriter w{ out };

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


void CamDetectShow::execute()
{
    io::Camera cam;
    io::VideoPlayer player{ "faces" };
    cv::Mat frame;

    algo::DetectorLBP detector{ "../res/haarcascades" };

    while( cam >> frame )
    {
        const auto rects = detector.get_face_rects( frame );
        io::draw_rects( frame, rects );
        player << frame;
    }
}


CamTrain::CamTrain( const std::string & label,  const std::string & fname_model )
    : _label{ label }
    , _model{ fname_model }
{
}


void CamTrain::execute()
{
    io::Camera cam{ io::Mode::_grayscale };
    io::VideoPlayer vid{ "training in progress..." };
    algo::DetectorLBP detector{ "../res/haarcascades" };
    cv::Mat frame;

    while( cam >> frame )
    {
        cv::equalizeHist( frame, frame );

        const auto rects = detector.get_face_rects( frame );
        io::draw_rects( frame, rects );
        vid << frame;

        if( rects.size() == 1 )
        {
            const auto face = io::crop( frame, rects[0] );
            _model.update( 0, face );
            _model.save();
        }
    }
}


struct CamRecognise::Impl
{
    Impl( const std::string & fname_model )
        : _recognizer{ cv::face::createLBPHFaceRecognizer() }
    {
        _recognizer->load( fname_model );
    }


    void run()
    {
        io::Camera cam{ io::Mode::_grayscale };
        algo::DetectorLBP detector{ "../res/haarcascades" };
        io::VideoPlayer vid{ "recognising in progress..." };
        cv::Mat frame;

        while( cam >> frame )
        {
            cv::equalizeHist( frame, frame );

            const auto rects = detector.get_face_rects( frame );
            io::draw_rects( frame, rects );
            vid << frame;

            for( const auto & r : rects )
            {
                double confidence;
                int label;
                const auto face = io::crop( frame, r );
                _recognizer->predict( face, label, confidence );
                std::cout << "DETECTED"
                          << " face number: " << std::to_string( label )
                          << ", confidence: " << std::to_string( confidence )
                          << std::endl;
            }
        }
    }


    cv::Ptr< cv::face::FaceRecognizer > _recognizer;
};


CamRecognise::CamRecognise( const std::string & fname_model )
    : _impl{ std::make_unique<Impl>( fname_model ) }
{
}


void CamRecognise::execute()
{
    _impl->run();
}


CamRecognise::~CamRecognise()
{
}

}  // namespace cmd
