#ifndef ENGINE_DRAWCALL_HPP
#define ENGINE_DRAWCALL_HPP

namespace engine
{

    /*
        This class can contain the data required for making basic drawcalls and uses virtual functions to enable more complex calls.
    */
    class DrawCall
    {
    public:
        //Hoe werkt deze constructor?

    protected:
        size_t m_triangles;
        
        GLuint m_texture;
        GLuint m_vertex_array;
        GLuint m_vertex_buffer;
        
        std::string m_string;
        math::Vector3<float> m_color;
        math::Matrix4x4<float> m_transform;
    };

}

#endif