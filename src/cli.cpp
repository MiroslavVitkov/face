#include "cli.h"

#include <opencv2/opencv.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


namespace cli
{


std::unique_ptr<cmd::Base> parse( int argc, Argv argv )
{
    const std::string keys =
        "{help h usage ? |      | print this message}"
        "{train t        |      | train from camera; -t=subject_name}"
        "{recognise r    |      | recognise from camera}"
        ;

    cv::CommandLineParser parser( argc, argv, keys );

    if( parser.has( "help" ) )
    {
        parser.printMessage();
        return std::make_unique<cmd::NoOp>();
    }

    if( parser.has( "train" ) )
    {
        const std::string label = parser.get<cv::String>( "train" );
        if( label.empty() || label == "true" )
        {
            std::cerr << "Error: training label not provided!\n";
            return std::make_unique<cmd::NoOp>();
        }

        return std::make_unique<cmd::CamTrain>( label );
    }

    if( parser.has( "recognise" ) )
    {
        return std::make_unique<cmd::CamRecognise>();
    }

    return std::make_unique<cmd::NoOp>();
}


}  // namespace cli
