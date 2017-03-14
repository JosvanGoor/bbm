#ifndef BBM_OBJECT_HPP
#define BBM_OBJECT_HPP

#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>
#include <exception>
#include "extern/gl_core_4_4.h"

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

std::vector<unsigned char> read_file(std::string file);

void print_if_opengl_error(std::string msg);
std::string translate_last_opengl_error();
std::string translate_opengl_error_code(GLenum error);

#endif