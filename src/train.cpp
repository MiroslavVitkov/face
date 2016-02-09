#include <dirent.h>

#include <opencv2/core.hpp>

#include <cassert>
#include <iostream>
#include <map>
#include <vector>


struct Images
{
    // Input data to the recogniser training algorithm.
    // Needs to be exactly in this format.
    std::vector<cv::Mat> images;
    std::vector<int> labels;

    // Mapping from numeric labels to string names of persons.
    using Labels2Strings = std::map<int, std::string>;
    Labels2Strings m;
};


// Images are expected to be distributed among sub-folders inside a single folder.
void read_images(const std::string &path, Images &out)
{
    // Go to selected path.
    DIR *dir;
    struct dirent *ent;
    dir = opendir(path.c_str());
    if( dir == NULL)
    {
        std::cerr << "Could not open target location.";
        std::cerr << "Make sure path to a directory was supplied as argv[1]." << std::endl;
        std::exit(-1);
    }

    // Iterate each subfolder.
  /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL)
    {
        printf ("%s\n", ent->d_name);
    }
    closedir (dir);
    // Record the name of the folder, map a numeric counter to it.
    // Read all images inside the folder.
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
