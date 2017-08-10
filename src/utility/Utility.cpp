#include "Utility.hpp"

namespace utility
{
    std::string trim(const std::string &str)
    {
        return trim_front(trim_back(str));
    }

    std::string trim_front(const std::string &str)
    {
        if(str.size() == 0) return "";
        return str.substr(str.find_first_not_of(" \t\n\r"), str.size());
    }

    std::string trim_back(const std::string &str)
    {
        if(str.size() == 0) return "";
        return str.substr(0, str.find_last_not_of(" \t\n\r") + 1);
    }

    std::string to_uppercase(const std::string &str)
    {
        std::string rval;
        rval.reserve(str.size());

        for(size_t i = 0; i < str.size(); ++i)
            rval += toupper(str[i]);
        
        return rval;
    }

    std::vector<std::string> split(const std::string &str, char s)
    {
        size_t pos = 0;
        std::vector<std::string> rval;

        while(true)
        {
            size_t pos2 = str.find_first_of(s, pos);

            if(pos2 == std::string::npos) //if end of line
            {
                //push final result, break
                rval.push_back(str.substr(pos, pos2));
                break;
            }

            //push part and increment past the splitchar.
            rval.push_back(str.substr(pos, pos2 - pos));
            pos = pos2 + 1;
        }

        rval.shrink_to_fit();
        return rval;
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

    std::string get_noncomment_line(std::istream &is, std::string commentstart)
    {
        std::string rval = get_line(is);
        while(std::equal(commentstart.begin(), commentstart.end(), rval.begin()))
        {
            rval = get_line(is);
        }
        return rval;
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
}