#ifndef ENGINE_SHADERPROGRAM_HPP
#define ENGINE_SHADERPROGRAM_HPP

#include <vector>
#include <memory>
#include "../core/Exception.hpp"
#include "../core/Stringable.hpp"
#include "../extern/gl_core_4_4.h"
#include "../utility/Utility.hpp"

namespace engine 
{

    class ShaderProgram : public Stringable
    {
    public:
        virtual ~ShaderProgram(); //public so its reachable for shared_ptr.

        void link();
        bool linked();
        void activate();

        void add_vertex_shader(std::string file);
        void add_fragment_shader(std::string file);

        GLuint program_id() const;
        GLint uniform_location(std::string name) const;

        virtual std::string to_string() const;
        
        //static create and removes
        static ShaderProgram* create();
        static std::shared_ptr<ShaderProgram> create_shared();
    protected:
        ShaderProgram();
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram(ShaderProgram&&) = delete;
        void operator=(const ShaderProgram&) = delete;
        void operator=(ShaderProgram&&) = delete;

        bool m_linked;
        GLuint m_vertex_shader;
        GLuint m_fragment_shader;
        GLuint m_shader_program;
    };

}

#endif