#include "view.hpp"
#include <iostream>
int main()
{
    view::Point up_left( view::X(9), view::Y(9) );
    view::Point down_right( view::X(20), view::Y(-2) );
    view::Rectangle window( up_left, down_right );
    std::cout <<  window.get_area() << "pixels area, center is at " << window.get_center();
    return 0;
}

/*


// While there are frames in 'cam', pass them through the pipeline.
while( cam )
{
    cam >> grayscale >> equalize >> show;
}


using Frame = cv::Mat;


struct FrameSource
{
    virtual Frame get() = 0;
    operator bool() const = 0;
};

struct FrameSink
{
    virtual void set( const Frame & ) = 0;
};

struct Procssor : FrameSink, FrameSource
{
};

FramSink & operator>>( FramSource & source, FramSink & sink )
{
    const auto frame = source.get();
    sink.set( frame );
    return sink;
}


FramSource & operator>>( FramSource & source, Processor & sink )
{
    const auto frame = source.get();
    sink.set( frame );
    return sink;
}


*/
