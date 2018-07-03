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

