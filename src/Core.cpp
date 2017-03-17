#include "Core.hpp"

#include <iterator>
#include <streambuf>

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
    return "Exception at " + m_source + ": " + m_message;
}

std::string get_line(std::istream& is)
{
    std::string rval;

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.
    // Adapted from: http://stackoverflow.com/a/6089413.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return rval;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return rval;
        case EOF:
            // Also handle the case when the last line has no line ending
            if(rval.empty())
                is.setstate(std::ios::eofbit);
            return rval;
        default:
            rval += (char)c;
        }
    }
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