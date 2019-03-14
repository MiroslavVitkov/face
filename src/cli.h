#ifndef CLI_H_
#define CLI_H_


#include "cmd.h"

#include <memory>


namespace cli
{


using Argc = int;
using Argv = const char *const *const;

std::unique_ptr<cmd::Base> parse( Argc argc, Argv argv );


}  // namespace cli


#endif  // defined(CLI_H_)
