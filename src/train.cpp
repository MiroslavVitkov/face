#include <dirent.h>

#include <opencv2/core.hpp>

#include <cassert>
#include <iostream>
#include <map>
#include <vector>


using namespace std;


struct Images
{
    // Input data to the recogniser training algorithm.
    // Needs to be exactly in this format.
    std::vector<cv::Mat> images;
    std::vector<int> labels;

    // Mapping from numeric labels to string names of persons.
    using Labels2Strings = std::map<int, std::string>;
    using MapElem = Labels2Strings::value_type;
    Labels2Strings maps;
};


// Returns true if entry is the unix current of parent folder.
bool skip_auto_dirs(struct dirent *ent)
{
    bool is_currdir = !std::string(ent->d_name).compare( std::string(".") );
    bool is_pardir  = !std::string(ent->d_name).compare( std::string("..") );
    if(is_currdir || is_pardir)  return true;
    else                         return false;
}


// Images are expected to be distributed among sub-folders inside a single folder.
void read_images(const std::string &path, Images &out)
{
    // Go to selected path.
    DIR *parent_dir = opendir(path.c_str());
    if( parent_dir == NULL)
    {
        std::cerr << "Could not open target location.";
        std::cerr << "Make sure path to a directory was supplied as argv[1]." << std::endl;
        std::exit(-1);
    }

    // Iterate each subfolder.
    struct dirent *ent, *ent2;
    while ( (ent = readdir(parent_dir)) != NULL )
    {
        if( skip_auto_dirs(ent) )  continue;

        DIR *dir = opendir((std::string(path.c_str()) + std::string(ent->d_name)).c_str());
        if(dir == NULL)
        {
            std::cerr << "Failed opening subdirectory " << ent->d_name << std::endl;
            continue;
        }

        // Record the name of the folder, map a numeric counter to it.
        unsigned counter = 0;
        out.labels.push_back(counter);
        out.maps.insert( Images::MapElem{counter, ent->d_name} );
        ++counter;

        // Read all images inside the folder.
        while( (ent2 = readdir(dir)) != NULL )
        {
            if( skip_auto_dirs(ent2) )  continue;
            printf("%s\n", ent2->d_name);
        }
        closedir(dir);
    }
    closedir(parent_dir);
}


int main(int argc, char **argv)
{
    // Get command-line parameters.
    assert(argc == 2);
    std::string path(argv[1]);

    Images images;
    read_images(path, images);

    return 0;
}
