#ifndef UTILITY_UTILITY_HPP
#define UTILITY_UTILITY_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "../extern/gl_core_4_4.h"

namespace utility
{

    std::string trim(const std::string &str);
    std::string trim_front(const std::string &str);
    std::string trim_back(const std::string &str);
    std::string to_uppercase(const std::string &str);

    std::vector<std::string> split(const std::string &str, char s);

    std::string get_line(std::istream &is);
    std::string get_noncomment_line(std::istream &is, std::string commentstart = "#");
    std::vector<unsigned char> read_file(std::string file);

    void print_if_opengl_error(std::string msg);
    std::string translate_last_opengl_error();
    std::string translate_opengl_error_code(GLenum error);

}

#endif