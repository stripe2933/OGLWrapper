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

        template <GLenum Target>
        Texture<Target> toTexture(bool generate_mipmap = true) const {
            Texture<Target> texture{};
            texture.bind();

            glTexImage2D(Target, 0, mapChannelToInternalFormat(channels), width, height, 0, mapChannelToFormat(channels), GL_UNSIGNED_BYTE, data);
            glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT);
            if (generate_mipmap) {
                glGenerateMipmap(Target);
            }

            return texture;
        }

        static Texture<GL_TEXTURE_CUBE_MAP> toCubemap(const Image &right, const Image &left, const Image &top, const Image &bottom, const Image &back, const Image &front);

        ~Image();
    };
}