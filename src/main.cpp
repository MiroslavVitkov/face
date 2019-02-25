#include "iface.h"

#include <iostream>

void test()
{
    DirReader d{ "dataset/putka" };
    cv::Mat face;
    while( d >> face )
    {
        std::cout << "new image\n";
    }
    return;

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
