#include "TextureCache.hpp"

namespace engine
{

    TextureCache::~TextureCache()
    {
        clear();
    }

    bool TextureCache::has_texture(const std::string &file)
    {
        return m_textures.find(file) != m_textures.end();
    }

    //This function fails silently, if no texture can be found or loaded it returns 0.
    GLuint TextureCache::get_texture(const std::string &file)
    {
        auto it = m_textures.find(file);
        if(it == m_textures.end()) return load_texture(file);
        return it->second;
    }

    GLuint TextureCache::load_texture(const std::string &file)
    {
        GLenum glerr;
        unsigned int width, height;
        std::vector<unsigned char> image;
        unsigned int error = lodepng::decode(image, width, height, file);

        if(error != 0) throw Exception(__PRETTY_FUNCTION__, "failed to read image - " + std::string(lodepng_error_text(error)) + ", " + file);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D,  texture);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glerr = glGetError();
        if(glerr != GL_NO_ERROR) throw Exception(__PRETTY_FUNCTION__, "failed to generate texture and setting parameters: " + translate_opengl_error_code(glerr));

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

        glerr = glGetError();
        if(glerr != GL_NO_ERROR) throw Exception(__PRETTY_FUNCTION__, "failed to load data into opengl texture object: " + translate_opengl_error_code(glerr));

        glBindTexture(GL_TEXTURE_2D, 0); //unbind texture.

        //remove any present texture before instering this (to prevent dangling textures).
        auto it = m_textures.find(file);
        if(it != m_textures.end())
        {
            glDeleteTextures(1, &(it->second));
            it->second = texture;
        }
        else m_textures.insert(std::pair<std::string, GLuint>(file, texture));

        return texture;
    }

    GLuint TextureCache::load_nonscaling_texture(const std::string &file)
    {
        GLenum glerr;
        unsigned int width, height;
        std::vector<unsigned char> image;
        unsigned int error = lodepng::decode(image, width, height, file);

        if(error != 0) throw Exception(__PRETTY_FUNCTION__, "failed to read image - " + std::string(lodepng_error_text(error)) + ", " + file);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D,  texture);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        glerr = glGetError();
        if(glerr != GL_NO_ERROR) throw Exception(__PRETTY_FUNCTION__, "failed to generate texture and setting parameters: " + translate_opengl_error_code(glerr));

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

        glerr = glGetError();
        if(glerr != GL_NO_ERROR) throw Exception(__PRETTY_FUNCTION__, "failed to load data into opengl texture object: " + translate_opengl_error_code(glerr));

        glBindTexture(GL_TEXTURE_2D, 0); //unbind texture.

        //remove any present texture before instering this (to prevent dangling textures).
        auto it = m_textures.find(file);
        if(it != m_textures.end())
        {
            glDeleteTextures(1, &(it->second));
            it->second = texture;
        }
        else m_textures.insert(std::pair<std::string, GLuint>(file, texture));

        return texture;
    }

    //does nothing (but waste time) when the entry doesn't exist.
    void TextureCache::remove_texture(const std::string &file)
    {
        auto it = m_textures.find(file);
        if(it == m_textures.end()) return;

        glDeleteTextures(1, &(it->second));
        m_textures.erase(file);
    }

    size_t TextureCache::size() const //returns number of loaded textures.
    {
        return m_textures.size();
    }


    void TextureCache::clear()
    {
        for(auto it : m_textures)
        {
            glDeleteTextures(1, &(it.second));
        }
        m_textures.clear();
    }

    void TextureCache::revalidate_all_textures()
    {
        for(auto it : m_textures)
        {
            load_texture(it.first);
        }
    }

    std::string TextureCache::to_string() const
    {
        return "Texture cache, currently containing " + std::to_string(m_textures.size()) + " textures.";
    }
}