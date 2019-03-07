#include "iface.h"

#include <iostream>

void test()
{



    auto cc = get_subdirs( "dataset", true );
    cv::Mat m;
    for( auto & stream : cc )
    {
        VideoWriter vw{ stream.get_label() + ".avi", stream.get_size() };
        std::cout << stream.get_label() << std::endl;

        while( stream >> m )
        {
            vw << m;
        }
    }
}

int main()
{
    test();
}
