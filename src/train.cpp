#include <dirent.h>

#include <opencv2/core.hpp>

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

    // Mapping from numeric ids to string names of persons.
    std::map<Id, std::string> _id_to_name;
};


// Returns true if entry is the unix current of parent directory(dir).
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
    if( parent_dir == NULL)
    {
        std::cerr << "Could not open target location.";
        std::cerr << "Make sure path to a directory was supplied as argv[1]." << std::endl;
        std::exit(-1);
    }

    // Iterate each subdir.
    unsigned i = 0;
    Images out;
    struct dirent *ent = NULL, *ent2 = NULL;
    while( (ent = readdir(parent_dir)) != NULL )
    {
        if( is_auto_dir(ent) )  continue;

        DIR *dir = opendir( (path + ent->d_name).c_str() );
        if(dir == NULL)
        {
            std::cerr << "Failed opening subdirectory " << ent->d_name << std::endl;
            continue;
        }

        // Read all images inside the dir.
        while( (ent2 = readdir(dir)) != NULL )
        {
            if( is_auto_dir(ent2) )  continue;
            std::cout << path << ent->d_name << '/' << ent2->d_name  // relative path
                      << ";"                                         // image-path;id separator
                      << i << std::endl;
        }
        closedir( dir );

        // Record the name of the dir, map a numeric counter to it.
        out._labels.push_back( i );
        out._id_to_name.emplace( i, ent->d_name );
        ++i;

    }
    closedir( parent_dir );

    return out;
}


int main( int argc, char **argv )
{
    // Get command-line parameters.
    if( argc != 2 )
    {
        std::cout << "Usage: " << argv[0]
                  << " path-to-dir-with-subdirs-with-images-for-each-person"
                  << std::endl;
        std::exit( -1 );
    }
    const std::string path( argv[1] );

    // Output to stdout a .csv file, usable by  opencv's training algorithms.
    read_images( path );

    return 0;
}
