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
    const cv::String keys =
        "{help h usage ? |      | print this message        }"
        "{test t         |      | diagnostics for debugging }"
        "{@image2        |<none>| image2 for compare   }"
        "{@repeat        |1     | number               }"
        "{path           |.     | path to file         }"
        "{fps            | -1.0 | fps for output video }"
        "{N count        |100   | count of objects     }"
        "{ts timestamp   |      | use time stamp       }"
        ;

    cv::CommandLineParser parser( argc, argv, keys );

    if( parser.has( "help" ) )
    {
        parser.printMessage();
        return std::make_unique<cmd::NoOp>();
    }

    if( parser.has( "test" ) )
    {
        return std::make_unique<cmd::Test>( cmd::Test::Case::_cam_to_vid
                                          , 100
                                          , "ignored"
                                          , "cam" );
    }

    //return std::make_unique<cmd::CamTrain>( 1, "", "model" );
    return std::make_unique<cmd::CamRecognise>();
}

/*
std::unique_ptr<cmd::Base> parse( Argc argc, Argv argv )
{
    // If '--help' is present, nothing else gets considered.
    for( int i = 1; i < argc; ++i)
    {
        const std::string help{ "--help" };
        const std::string h{ "-h" };

        if( ( help.compare( argv[i] ) == 0 ) || ( h.compare( argv[i] ) == 0 ) )
        {
            return std::unique_ptr<cmd::Base>{ new cmd::PrintHelp };
        }
    }

    // Because output file can be given as '-o filename' or as '-ofilename',
    // we need ot filter the parameter list,
    // before atempting to extract the command form it.
    std::string fname_out {};
    std::vector<std::string> without_options;
    bool next_is_ofile = false;
    for( int i = 1; i < argc; ++i)
    {
        const std::string o{ "-o" };
        const std::string outfile{ "--outfile" };

        // Handle -ofilename case.
        if( argv[i][0] == '-' && argv[i][1] == 'o' && argv[i][2] != '\0' )
        {
            fname_out.assign( argv[i] + 2 );
            continue;
        }
        if( std::equal( outfile.begin(), outfile.end(), argv[i] ) )
        {
            fname_out.assign( argv[i] + outfile.size() );
            continue;
        }

        // Handle '-o filename' case.
        if( next_is_ofile )
        {
            fname_out.assign( argv[i] );
            next_is_ofile = false;
            continue;
        }
        if( ( outfile.compare( argv[i] ) == 0 ) || ( o.compare( argv[i] ) == 0 ) )
        {
            next_is_ofile = true;
        }

        without_options.emplace_back( std::string{ argv[i] } );

    }
    if( next_is_ofile )
    {
        std::cerr << "Error: -o flag needs to be followed by a filename arguement!\n";
        return std::unique_ptr<cmd::Base>{ new cmd::PrintHelp };
    }

    // Now try to extract the 'COMMAND FILENAME' part.
    if( without_options.size() != 2 )
    {
        std::cerr << "Error: please provide exactly two non-optional arguements.\n";
        return std::unique_ptr<cmd::Base>{ new cmd::PrintHelp };
    }

    const std::string view{ "view" };
    const std::string region{ "region" };
    if( view.compare( without_options[0] ) )
    {
//        return std::unique_ptr<cmd::Base>{ new cmd::ViewOrSave{ without_options[1]
//                                                              , fname_out } };
    }
    else if( region.compare( without_options[0] ) )
    {
        std::cerr << "Error: Sorry, this command is not supported yet.\n";
        return std::unique_ptr<cmd::Base>{ new cmd::PrintHelp };
    }
    else
    {
        std::cerr << "Error: Requested command is not recognised.\n";
        return std::unique_ptr<cmd::Base>{ new cmd::PrintHelp };
    }
}

*/
}  // namespace cli
