// Detect faces in a video file or a usb camera.
// Output cropped faces on stdout or in an image folder.
//
// build-deps: libopencv-dev
// run-deps: haarcascade XMLs


#include "iface.h"

#include <queue>
#include <iostream>
#include <sstream>


cv::VideoCapture create_stream( const LBP::Settings & s )
{
    const auto & v = s.video_file;

    auto vid = [ &v ] ()
    {
        if( v.empty() )
        {
            return cv::VideoCapture( 0 );
        }
        else
        {
            return cv::VideoCapture( v );
        }
    } ();

    if( ! vid.isOpened() )
    {
        Exception e{ "Failed to open input video stream: " + v };
        throw e;
    }

    return vid;
}

LBP::LBP( const Settings & s )
: _settings{ s }
, _video_stream{ create_stream( s ) }
, _classifier{ create_classifier( s, "haarcascade_frontalface_alt") }
, _detected_faces{}
, _facerect_buff{}
{
    // todo: consider turning '_classifier' into a verctor
    // and adding at least 'haarcascade_eye_tree_eyeglasses'.
}


    cv::Mat get_one_face()
    {
        cv::Mat frame;

        // Block untll a frame with faces is seen.
        while( _facerect_buff.empty() )
        {
            _video_stream >> frame;
            assert( ! frame.empty() );
            detect( frame );
        }

        // Crop faces out of the video frame.
        for( const auto & f : _facerect_buff )
        {
            const cv::Mat cropped = frame( f );
            _detected_faces.push( cropped );
        }
        _facerect_buff.clear();

        // Return a face.
        const auto ret = _detected_faces.front();
        _detected_faces.pop();
        return ret;
    }



    void record_show_video()
    {
        cv::CascadeClassifier eyes_cascade( _settings.cascades_dir + "/haarcascade_eye_tree_eyeglasses.xml");
        assert( ! eyes_cascade.empty() );

            cv::Mat test;
            _video_stream >> test;
//        cv::VideoWriter vw( "kur.avi", cv:: VideoWriter::fourcc('P','I','M','1'), 20, test.size() );
        cv::VideoWriter vw( "kur.avi", cv:: VideoWriter::fourcc('M','J','P','G'), 10, test.size() );

        while(true)
        {
            cv::Mat frame;
            _video_stream >> frame;
              vw << frame;
          detect( frame );
            for( const auto & f : _facerect_buff )
            {
                cv::Point center( f.x + f.width/2, f.y + f.height/2 );
                cv::ellipse( frame, center, cv::Size( f.width/2, f.height/2 ), 0, 0, 360, cv::Scalar( 255, 0, 255 ), 4 );

                //-- In each face, detect eyes
                std::vector<cv::Rect> eyes;
                cv::Mat face{ frame, f };
                assert( ! face.empty() );
                eyes_cascade.detectMultiScale( face, eyes );
                for ( const auto & e : eyes )
                {
                    cv::Point eye_center( f.x + e.x + e.width/2, f.y + e.y + e.height/2 );
                    int radius = cvRound( (e.width + e.height)*0.25 );
                    cv::circle( frame, eye_center, radius, cv::Scalar( 255, 0, 0 ), 4 );
                }
            }
            _facerect_buff.clear();

            cv::imshow( "Capture - Face detection", frame );
            cv::waitKey(25);
        }
    }


private:



    static cv::CascadeClassifier create_classifier( const Settings & s
                                                  , const std::string & name )
    {
        const auto xml_path = s.cascades_dir + '/' + name + ".xml";
        cv::CascadeClassifier c( xml_path );
        if( c.empty() )
        {
            Exception e{ "Failed to load cascade classifier: " + xml_path };
        }

        return c;
    }


    void detect( const cv::Mat & in )
    {
        assert( ! in.empty() );
        assert( _facerect_buff.empty() );

        cv::Mat gray;

        // Convert to 8-bit single channel image.
        cv::cvtColor( in, gray, CV_BGR2GRAY );

        // Increase contrast in under- or over-exposed areas of the image.
        cv::equalizeHist( gray, gray );

        _classifier.detectMultiScale( gray, _facerect_buff );
        // Perhaps do low-pass filtering over 3 consecutive frames to evade false positives.
    }


};  // struct LBP


const LBP::Settings settings_test{ .cascades_dir{ "/usr/share/opencv/haarcascades" }
                            , .video_file{ "kur.mp4" }
                            , .output_dir{} };


const LBP::Settings settings_run{ .cascades_dir{ "/usr/share/opencv/haarcascades" }
                           , .video_file{}
                           , .output_dir{} };


int main( int, char ** )
{
    LBP alg{ settings_run };
    alg.record_show_video();
}
