// Does stuff.
//
// build-deps: libopencv-dev
// run-deps


#include <opencv2/opencv.hpp>

#include <dirent.h>

#include <cassert>
#include <iostream>
#include <map>
#include <vector>


struct Images
{
    using Id = int;

    // Input data to the recogniser training algorithm.
    // Needs to be exactly in this format.
    std::vector<cv::Mat> _images;
    std::vector<Id> _labels;
    std::map<Id, std::string> _id_to_name;
};


// True if entry is the unix current of parent directory.
bool is_auto_dir( struct dirent *ent )
{
    bool is_currdir = ! std::string( ent->d_name ).compare( "." );
    bool is_pardir  = ! std::string( ent->d_name ).compare( ".." );
    if( is_currdir || is_pardir )  return true;
    else                           return false;
}


// Images are expected to be distributed among sub-dirs inside a single dir.
// One subdir per person.
Images read_images( std::string path )
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


/*cv::Ptr<cv::FaceRecognizer> create_model( const Images &im )
{
//    auto model = cv::createLBPHFaceRecognizer();
 auto model = cv::createLBPHFaceRecognizer();
    model.train( im._images, im._labels );
    model.setLabelsInfo( im._id_to_name );
};


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

    auto im = read_images( imm_path );
//    auto model = create_model( im );
//    serialize_to_file( model, out_path );

    return 0;
}
