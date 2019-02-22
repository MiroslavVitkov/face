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



void record_show_video()
{
    cv::CascadeClassifier eyes_cascade( _settings.cascades_dir + "/haarcascade_eye_tree_eyeglasses.xml");
    assert( ! eyes_cascade.empty() );

        cv::Mat test;
        _video_stream >> test;
//        cv::VideoWriter vw( "kur.avi", cv:: VideoWriter::fourcc('P','I','M','1'), 20, test.size() );
    cv::VideoWriter vw( "kur.avi", cv:: VideoWriter::fourcc('M','J','P','G'), 10, test.size() );

    while(true)
    {
        cv::Mat frame;
        _video_stream >> frame;
          vw << frame;
      detect( frame );
        for( const auto & f : _facerect_buff )
        {
            cv::Point center( f.x + f.width/2, f.y + f.height/2 );
            cv::ellipse( frame, center, cv::Size( f.width/2, f.height/2 ), 0, 0, 360, cv::Scalar( 255, 0, 255 ), 4 );

            //-- In each face, detect eyes
            std::vector<cv::Rect> eyes;
            cv::Mat face{ frame, f };
            assert( ! face.empty() );
            eyes_cascade.detectMultiScale( face, eyes );
            for ( const auto & e : eyes )
            {
                cv::Point eye_center( f.x + e.x + e.width/2, f.y + e.y + e.height/2 );
                int radius = cvRound( (e.width + e.height)*0.25 );
                cv::circle( frame, eye_center, radius, cv::Scalar( 255, 0, 0 ), 4 );
            }
        }
        _facerect_buff.clear();

        cv::imshow( "Capture - Face detection", frame );
        cv::waitKey(25);
    }
}


const LBP::Settings settings_test{ .cascades_dir{ "/usr/share/opencv/haarcascades" }
                        , .video_file{ "kur.mp4" }
                        , .output_dir{} };


const LBP::Settings settings_run{ .cascades_dir{ "/usr/share/opencv/haarcascades" }
                       , .video_file{}
                       , .output_dir{} };


int main( int, char ** )
{
LBP alg{ settings_run };
alg.record_show_video();
}

