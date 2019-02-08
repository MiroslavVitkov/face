// Detect faces in a video file or a usb camera.
// Outputs cropped faces on stdout or in an image folder.
//
// build-deps: libopencv-dev
// run-deps: haarcascade XMLs


#include <opencv2/opencv.hpp>

#include <queue>
#include <exception>
#include <iostream>
#include <sstream>


struct Settings
{
    const std::string cascades_dir;
    const std::string video_file;  // empty -> capture from camera
    const std::string output_dir;  // empty -> stdout
};


Settings settings_test{ .cascades_dir{ "/usr/share/opencv/haarcascades" }
                      , .video_file{ "kur.mp4" }
                      , .output_dir{} };


Settings settings_run{ .cascades_dir{ "/usr/share/opencv/haarcascades" }
                      , .video_file{}
                      , .output_dir{} };


struct Exception : public std::runtime_error
{
    Exception( const std::string & msg )
    : std::runtime_error( msg )
    {
    }
};


struct Algorithm
{
    const Settings & _settings;
    cv::VideoCapture _video_stream;
    cv::CascadeClassifier _classifier;
    std::queue<cv::Mat> _faces_buff;

    Algorithm( const Settings & s )
    : _settings{ s }
    , _video_stream{ create_stream( s ) }
    , _classifier{ create_classifier( s, "haarcascade_frontalface_alt") }
    , _faces_buff{}
    {
        // todo: consider turning '_classifier' into a verctor
        // and adding at least 'haarcascade_eye_tree_eyeglasses'.
    }


    cv::Mat get_one_face()
    {
        while( _faces_buff.empty() )
        {
            cv::Mat frame;
            _video_stream >> frame;
            assert( ! frame.empty() );
            //crop_face();
            //push_to_buff();
        }

        const auto ret = _faces_buff.front();
        _faces_buff.pop();
        return ret;
    }


private:
    static cv::VideoCapture create_stream( const Settings & s )
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


};  // class Algorthm


int main( int argc, char *argv[] )
{
    Algorithm alg{ settings_run };
    const auto f = alg.get_one_face();
}


/*
    while( true )
    {
        cv::Mat frame;
        cap >> frame;
        assert( ! frame.empty() );
        detectAndDisplay( frame );
    }

    return 0;
}

void detectAndDisplay( cv::Mat frame )
{
  std::vector<cv::Rect> faces;
  cv::Mat frame_gray;

  cv::cvtColor( frame, frame_gray, CV_BGR2GRAY );
  cv::equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );

  // If there are any faces in the picture, count to 3. If this is the third in a row,
  // record the picture. This protects from false positives.
  static unsigned consecutive_frames_with_faces = 0;
  if(faces.empty())
  {
    consecutive_frames_with_faces = 0;
  } else
  {
    consecutive_frames_with_faces++;
    std::cout << faces.size() << " faces in image, image_count is " << consecutive_frames_with_faces << std::endl;

    static unsigned picture_number = 0;
    if(consecutive_frames_with_faces >= 3)
    {
      // Form file name.
      std::stringstream ss;
      ss << pictures_dir << "/face_" << picture_number++ << ".jpg";
      std::cout << "Writing picture " << ss.str() << std::endl;

      // Crop the faces from the picture and write them to a file.
      for(size_t i = 0; i < faces.size(); ++i)
      {
        cv::Rect myROI(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
        cv::Mat cropped = frame(myROI);
        cv::imwrite(ss.str(), cropped);
      }  // for each face in image
    }  // if lpf
  }  // if any faces in image
}  // foo()
*/
