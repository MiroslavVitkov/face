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
    // Iterate each subfolder.
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
