#ifndef EXCEPT_H_
#define EXCEPT_H_


#include <stdexcept>
#include <string>
#include <vector>


struct Exception : public std::runtime_error
{
    Exception( const std::string & msg )
        : std::runtime_error( msg ) { }
};


#endif // defined(EXCEPT_H_)
