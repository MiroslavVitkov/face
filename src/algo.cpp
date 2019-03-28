// Detect faces in a video file or a usb camera.
// Output cropped faces on stdout or in an image folder.
//
// build-deps: libopencv-dev
// run-deps: haarcascade XMLs


#include "algo.h"

#include <queue>
#include <iostream>
#include <sstream>


namespace algo
{


cv::CascadeClassifier create_classifier( const std::string & cascades_dir
                                       , const std::string & name )
{
    const auto xml_path = cascades_dir + '/' + name + ".xml";
    cv::CascadeClassifier c( xml_path );
    if( c.empty() )
    {
        Exception e{ "Failed to load cascade classifier: " + xml_path };
    }

    return c;
}


DetectorLBP::DetectorLBP( const std::string & cascades_dir )
    :_classifier{ create_classifier( cascades_dir, "haarcascade_frontalface_alt" ) }
{
    // todo: consider turning '_classifier' into a verctor
    // and adding at least 'haarcascade_eye_tree_eyeglasses'.
}


std::vector<cv::Rect> DetectorLBP::get_face_rects( const cv::Mat & frame
                                                 , double min_confidence
                                                 )
{
    assert( ! frame.empty() );

    // Increase contrast in under- or over-exposed areas of the image.
    cv::equalizeHist( frame, frame );

    std::vector<cv::Rect> rects;
    _classifier.detectMultiScale( frame, rects );
    // Perhaps do low-pass filtering over 3 consecutive frames to evade false positives.
    (void)min_confidence;

    return rects;
}


std::vector<cv::Mat> DetectorLBP::get_faces( const cv::Mat & frame
                                           , double min_confidence
                                           )
{
    const auto rects = get_face_rects( frame, min_confidence );

    // Crop faces out of the video frame.
    std::vector<cv::Mat> faces;
    for( const auto & r : rects )
    {
        const auto cropped = frame( r );
        faces.push_back( cropped );
    }

    return faces;
}


int label_to_number( const std::string & label
                   , const cv::Ptr<cv::face::FaceRecognizer> model )
{
    // Look for existing entry.
    const auto nums = model->getLabelsByString( label );
    if( nums.size() == 1 )
    {
        return nums[0];
    }
    else if( nums.size() > 1 )
    {
        throw Exception{ "One to one relationship between string and numeric "
                         "labels violated." };
    }

    // Find the next unoccupied entry.
    for( int i = 0; ; ++i )
    {
        const auto str = model->getLabelInfo( i );
        if( str.empty() )
        {
            return i;
        }
    }
}


TrainerLBP::TrainerLBP( const std::string & label
                      , const std::string & fname_model )
    : _model{ cv::face::createLBPHFaceRecognizer() }
    , _fname_model{ fname_model }
    , _tmp_labels{}
    , _tmp_faces{}
{
    try
    {
        _model->load( fname_model );
    }
    catch( ... )
    {
        // File doesn't exist - load nothing.
    }

    const auto l = label_to_number( label, _model );
    _model->setLabelInfo( l, label );

    _tmp_labels.push_back( l );
    _tmp_faces.emplace_back( cv::Mat{} );
}

void TrainerLBP::update( const cv::Mat & gray_face )
{
    _tmp_faces[0] = gray_face;
    _model->update( _tmp_faces, _tmp_labels );
}


void TrainerLBP::save() const
{
    _model->save( _fname_model );
}



int predict( const cv::Mat & gray_face );


}  // namespace algo
