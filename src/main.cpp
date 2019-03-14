#include "cli.h"


int main( int argc, cli::Argv argv )
{
    const auto action = cli::parse( argc, argv );
    action->execute();

    return 0;
}
