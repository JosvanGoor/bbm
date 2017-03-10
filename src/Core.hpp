#ifndef BBM_OBJECT_HPP
#define BBM_OBJECT_HPP

#include <string>
#include <iostream>
#include <typeinfo>
#include <exception>

class Object
{
public:
    virtual std::string to_string() const = 0;
    virtual std::string class_name() const;
};

std::ostream& operator<<(std::ostream &os, const Object &obj);

class Exception : public std::exception
{
public:
    Exception(const std::string &source, const std::string &msg);

    virtual const char* what() const noexcept;
    virtual std::string message() const;

protected:
    Exception();
    std::string m_source;
    std::string m_message;
};

#endif