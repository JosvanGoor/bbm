#ifndef CORE_EXCEPTION_HPP
#define CORE_EXCEPTION_HPP

#include <string>
#include <exception>

class Exception : public std::exception
{
    public:
        Exception(const std::string &src, const std::string &msg);

        virtual const std::string &message() const;
        virtual const char *what() const noexcept override;

    protected:
        std::string m_message;
};

#endif