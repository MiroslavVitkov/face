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
            cv::imwrite("kur.jpg", m);
            vw << m;
        }

//        do
//        {
//            stream >> m;
//            vw << m;
//        }
//        while(stream);
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
