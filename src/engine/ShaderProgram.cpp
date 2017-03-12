#include "ShaderProgram.hpp"

namespace engine 
{

    ShaderProgram::ShaderProgram()
    {
        m_linked = false;
        m_vertex_shader = 0;
        m_fragment_shader = 0;
        m_shader_program = glCreateProgram();
    }
    
    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_shader_program);
        glDeleteShader(m_fragment_shader);
        glDeleteShader(m_vertex_shader);
    }

    void ShaderProgram::link()
    {
        if(m_vertex_shader) glAttachShader(m_shader_program, m_vertex_shader);
        if(m_fragment_shader) glAttachShader(m_shader_program, m_fragment_shader);
        glLinkProgram(m_shader_program);

        GLint success;
        glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
        if (success == 0)
        {
            GLchar info[1024];
            glGetProgramInfoLog(m_shader_program, sizeof(info), NULL, info);
            throw Exception("ShaderProgram::link", std::string(info));
        }
        
        m_linked = true;
    }

    bool ShaderProgram::linked()
    {
        return m_linked;
    }

    void ShaderProgram::activate()
    {
        if(!m_linked) link();
        glUseProgram(m_shader_program);

        GLenum error = glGetError();
        if(error != GL_NO_ERROR) throw Exception("ShaderProgram::activate", "Failed to activate shader program - " + translate_opengl_error_code(error));
    }

    void ShaderProgram::add_vertex_shader(std::string file)
    {
        if(m_vertex_shader)
        {
            //delete any previous program.
            if(m_linked) glDetachShader(m_shader_program, m_vertex_shader);
            glDeleteShader(m_vertex_shader);
        }
        
        m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        std::vector<unsigned char> data = read_file(file);
        const char* pbuf = reinterpret_cast<char*>(data.data());
        int size = data.size();

        glShaderSource(m_vertex_shader, 1, &pbuf, &size);
        glCompileShader(m_vertex_shader);

        GLint success;
        glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            GLchar info[1024];
            glGetShaderInfoLog(m_vertex_shader, sizeof(info), NULL, info);
            throw Exception("ShaderProgram::add_vertex_shader", std::string(info));
        }
    }


    void ShaderProgram::add_fragment_shader(std::string file)
    {
        glDeleteShader(m_fragment_shader); //delete any previous program.
        
        m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        std::vector<unsigned char> data = read_file(file);
        const char* pbuf = reinterpret_cast<char*>(data.data());
        int size = data.size();

        glShaderSource(m_fragment_shader, 1, &pbuf, &size);
        glCompileShader(m_fragment_shader);

        GLint success;
        glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            GLchar info[1024];
            glGetShaderInfoLog(m_fragment_shader, sizeof(info), NULL, info);
            throw Exception("ShaderProgram::add_fragment_shader", std::string(info));
        }
    }

    GLuint ShaderProgram::program_id() const
    {
        return m_shader_program;
    }


    GLint ShaderProgram::uniform_location(std::string name) const
    {
        return glGetUniformLocation(m_shader_program, name.c_str());
    }

    std::string ShaderProgram::to_string() const
    {
        return "Opengl shader program - " + m_linked ? std::string("currently not linked") : std::string("linked");
    }

}