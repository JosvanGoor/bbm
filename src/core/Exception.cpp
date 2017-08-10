#include "Exception.hpp"

Exception::Exception(const std::string &src, const std::string &msg)
{
    m_message = "Exception from " + src + ": " + msg;
}

const std::string &Exception::message() const
{
    return m_message;
}

const char *Exception::what() const noexcept
{
    return m_message.c_str();
}