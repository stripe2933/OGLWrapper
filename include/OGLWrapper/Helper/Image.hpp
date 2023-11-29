//
// Created by gomkyung2 on 11/26/23.
//

#pragma once

#include <OGLWrapper/Texture.hpp>

namespace OGLWrapper::Helper {
    class Image{
        static GLint mapChannelToInternalFormat(int channels);
        static GLenum mapChannelToFormat(int channels);

    public:
        int width;
        int height;
        int channels;
        unsigned char *data = nullptr;

        explicit Image(const char * source, bool flip = true);
        Image(const Image&) = delete;
        Image(Image&& source) noexcept;

        Texture toTexture(const TextureParameter &parameter, bool generate_mipmap = true) const;

        ~Image();
    };
}