#ifndef CORE_STRINGABLE_HPP
#define CORE_STRINGABLE_HPP

#include <string>

/*
    Defines a common interface for a to_string function. 
    Also provides a ostream<< overload.
*/

class Stringable
{
    public:
        virtual std::string to_string() const = 0;

        virtual ~Stringable() {};

};

std::ostream& operator<<(std::ostream &os, const Stringable &s);

#endif