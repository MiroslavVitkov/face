#ifndef VIEW_HPP_
#define VIEW_HPP_


#include <ostream>


namespace view
{


using Pixels = int;


struct Axis
{
    constexpr Axis() : _value( 0 ) {}
    constexpr Axis( Pixels n ) : _value( n ) {}

    const Pixels _value;
};


struct X : Axis { /*X( Pixels x ) : Axis( x ) {}*/ };
struct Y : Axis {};


struct Point
{
    constexpr explicit Point( X x, Y y )
        : _x( x ), _y( y ) {}

    std::ostream& print( std::ostream &out ) const
        { out << "point(x=" << _x << ",y=" << _y << ")"; }

    const X _x;
    const Y _y;
};



struct Rectangle
{
    constexpr explicit Rectangle( Point p1, Point p2 )
        : _p1( p1 ), _p2( p2 ) {}
    std::ostream& print( std::ostream &out )
        { out << "rectangle(" << _p1 << "," << _p2 << ")"; }

    Pixels get_area() const {}
    Point get_center() const {}

private:
    const Point _p1, _p2;
};


std::ostream& operator<<( std::ostream& s, const Point &p )
    { return p.print( s ); }

std::ostream& operator<<( std::ostream& s, const Rectangle &r )
    { return r.print( s ); }


}  // namespace view


#endif  // VIEW_HPP_
