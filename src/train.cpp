// Does stuff.
//
// build-deps: libopencv-dev
// run-deps


#include <opencv2/opencv.hpp>

#include <iostream>
#include <map>
#include <vector>




using namespace cv;
//Ptr<FaceRecognizer> create_model( const Images &im )
//{
//    auto model = createLBPHFaceRecognizer();
//    model.train( im._images, im._labels );
//    model.setLabelsInfo( im._id_to_name );
//};

/*
void serialize_to_file( cv::Ptr<cv::FaceRecognizer> model, std::string fname )
{
    model->save( fname );
}
*/

int main( int argc, char **argv )
{
    // Get command-line parameters.
    if( argc != 3 )
    {
        std::cout << "Usage: " << argv[0]
                  << " path-to-dir-with-subdirs-with-images-for-each-person"
                  << " path-for-output-file"
                  << std::endl;
        std::exit( -1 );
    }
    const std::string imm_path( argv[1] );
    const std::string out_path( argv[2] );

//    auto im = read_images( imm_path );
//    auto model = create_model( im );
//    serialize_to_file( model, out_path );

    return 0;
}
