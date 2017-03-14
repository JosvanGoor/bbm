#include "Core.hpp"

#include <fstream>
#include <iterator>

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

std::vector<unsigned char> read_file(std::string file)
{
    std::ifstream in(file.c_str(), std::ios::binary);

    //get size of file so we can pre-allocate memory.
    in.seekg(0, std::ios::end);
    std::streampos size = in.tellg();
    in.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer;
    buffer.reserve(size);
    buffer.insert(
        buffer.begin(),
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>());
    
    return buffer;
}

std::string translate_opengl_error_code(GLenum error)
{
    switch(error)
    {
        case GL_NO_ERROR: return "GL_NO_ERROR";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        default: return "UNKNOWN_ERROR";
    }
}

std::string translate_last_opengl_error()
{
    return translate_opengl_error_code(glGetError());
}

void print_if_opengl_error(std::string msg)
{
    GLenum err = glGetError();
    if(err == GL_NO_ERROR) return;
    std::cout << msg << " - " << translate_opengl_error_code(err) << std::endl;
}