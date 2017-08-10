#include "Stringable.hpp"

std::ostream& operator<<(std::ostream &os, const Stringable &s)
{
    os << s.to_string();
    return os;
}