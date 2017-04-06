#ifndef BBM_ENGINE_TEXTURECACHE_HPP
#define BBM_ENGINE_TEXTURECACHE_HPP

#include <map>
#include "../Core.hpp"
#include "../extern/lodepng.h"

namespace engine
{

    class TextureCache : public Object
    {
    public:
        TextureCache() = default;
        TextureCache(const TextureCache&) = delete;
        TextureCache(const TextureCache&&) = delete;
       ~TextureCache();

        bool has_texture(const std::string &file);
        GLuint get_texture(const std::string &file);
        GLuint load_texture(const std::string &file);
        void remove_texture(const std::string &file);

        GLuint load_nonscaling_texture(const std::string &file);

        size_t size() const; //returns number of loaded textures.
        void clear(); //removes all textures.
        void revalidate_all_textures(); //reloads all textures (by key).

        virtual std::string to_string() const;
    protected:
        std::map<std::string, GLuint> m_textures;
    };

}

#endif
