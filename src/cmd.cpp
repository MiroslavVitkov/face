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


void draw_rect( cv::Mat & frame, cv::Rect rect )
{
    cv::rectangle( frame, rect, cv::Scalar(0, 255, 0) );
}


void CamDetectShow::execute()
{
    io::Camera cam;
    io::VideoPlayer player{ "faces" };
    cv::Mat frame;

    DetectorLBP detector{ "../res/haarcascades" };

    while( cam >> frame )
    {
        const auto rects = detector.get_face_rects( frame );
        for( const auto & r : rects )
        {
            draw_rect( frame, r );
        }

        player << frame;
    }
}


struct CamTrain::Impl
{
    Impl( int label 
        , const std::string & fname_model_in
        , const std::string & fname_model_out )
        : _label{ label }
        , _recognizer{ cv::face::createLBPHFaceRecognizer() }
        , _fname_model_out{ fname_model_out }
    {
        if( ! fname_model_in.empty() )
        {
            _recognizer->load( fname_model_in );
        }
    }


    void update( const cv::Mat & face )
    {
        std::vector<cv::Mat> faces{ face };
        std::vector<int> labels{ _label };
        assert( faces.size() == labels.size() );

        _recognizer->update( faces, labels );
    }


    void execute()
    {
        io::Camera cam{ io::Mode::_grayscale };
        io::VideoPlayer vid{ "training in progress..." };
        cv::Mat frame;

        DetectorLBP detector{ "../res/haarcascades" };

        while( cam >> frame )
        {
            const auto rects = detector.get_face_rects( frame );
            for( const auto & r : rects )
            {
                draw_rect( frame, r );
            }
            vid << frame;

            const auto faces = detector.get_faces( frame );
            if( faces.empty() )
            {
            }
            else if( faces.size() > 1 )
            {
                std::cout << "Too many faces detected in frame: "
                           + std::to_string( faces.size() ) << std::endl;
            }
            else
            {
                update( faces.front() );
                std::cout << "Wrote model to file: " << _fname_model_out
                          << std::endl;
                _recognizer->save( _fname_model_out );
            }
        }
    }


    ~Impl()
    {
        _recognizer->save( _fname_model_out );
    }


private:
    int _label;
    cv::Ptr< cv::face::FaceRecognizer > _recognizer;
    const std::string _fname_model_out;
};


CamTrain::CamTrain( int label
                  , const std::string & fname_model_in
                  , const std::string & fname_model_out )
    : _impl{std::make_unique<Impl>( label, fname_model_in, fname_model_out ) }
{
}


void CamTrain::execute()
{
    _impl->execute();
}


CamTrain::~CamTrain()
{
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
        DetectorLBP detector{ "../res/haarcascades" };
        io::VideoPlayer player{ "debug view" };
        cv::Mat frame;

        while( cam >> frame )
        {
            const auto rects = detector.get_face_rects( frame );
            for( const auto & r : rects )
            {
                draw_rect( frame, r );
            }
            player << frame;

            const auto faces = detector.get_faces( frame );
            for( const auto & f : faces )
            {
                double confidence;
                int label;
                _recognizer->predict( f, label, confidence );
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
