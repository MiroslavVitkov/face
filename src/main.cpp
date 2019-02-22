#include "iface.h"

#include <iostream>

void test()
{


    const auto cc = get_subdirs( "dataset" );

    for( auto & stream : cc )
    {
        std::cout << stream.get_label() << std::endl;
//        cv::Mat m;
//        while( stream >> m )
//        {
//            std::cout << (m.data == nullptr) << ' ' << (! c) << std::endl;
//        }
    }
}

int main()
{
    test();
}
