#include "cmd.h"

#include <iostream>


namespace cmd
{

void PrintHelp::execute()
{
    std::cout << "Usage: aeolus [OPTIONS] [COMMAND FILE]\n"
                 "\n"
                 "This is a small image processing tool. "
                 "Currently, only PNG input and output files are supported.\n"
                 "\n"
                 "Commands:\n"
                 "view                - Show selected image file on screen, or copy it if -o is pecified.\n"
                 "region <X Y [DIST]> - Compute a same-colour region around coordinate.\n"
                 "                      DIST is (r1-r2)^2 + (g1-g2)^2 + (b1-b2)^2, default 0\n"
                 "\n"
                 "Option          GNU long option        Meaning\n"
                 "-h              --help                 Print this message and exit.\n"
                 "-o <filename>   --outfile <filename>   Write results to file; if omitted, result is shown on screen.\n";
}


}  // namespace cmd
