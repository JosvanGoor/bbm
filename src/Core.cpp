#include "Core.hpp"

std::string Object::class_name() const
{
    return typeid(*this).name();
}

std::ostream& operator<<(std::ostream &os, const Object &obj)
{
    os << obj.to_string();
    return os;
}

Exception::Exception() : m_source("unknown"), m_message("no message") {}
Exception::Exception(const std::string &source, const std::string &message) : m_source(source), m_message(message) {}

const char* Exception::what() const noexcept
{
    return message().c_str();
}

std::string Exception::message() const
{
    return "From [" + m_source + "], Message: " + m_message;
}
