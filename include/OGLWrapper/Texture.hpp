//
// Created by gomkyung2 on 11/26/23.
//

#pragma once

#include <stdexcept>

#include <GL/gl3w.h>

#include "strict_mode.hpp"

namespace OGLWrapper{
    struct TextureParamter {
        GLint wrap_s = GL_REPEAT;
        GLint wrap_t = GL_REPEAT;
        GLint min_filter = GL_LINEAR_MIPMAP_LINEAR;
        GLint mag_filter = GL_LINEAR;

        void setup() const {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

        }
    };

    class Texture {
        static bool isValidInternalFormat(GLint internal_format);
        static bool isValidFormat(GLenum format);
        static bool isValidType(GLenum type);

    public:
        GLuint handle;

        template <bool CheckFormat = STRICT_MODE>
        Texture(GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data, const TextureParamter &parameter = {}, bool generate_mipmap = true) {
            if constexpr (CheckFormat) {
                // See https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
                // TODO: add more formats.
                if (!isValidInternalFormat(internal_format))
                    throw std::invalid_argument { "Invalid internal format" };
                if (!isValidFormat(format))
                    throw std::invalid_argument { "Invalid format" };
                if (!isValidType(type))
                    throw std::invalid_argument { "Invalid type" };
            }

            glGenTextures(1, &handle);

            glBindTexture(GL_TEXTURE_2D, handle);
            parameter.setup();

            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, data);
            if (generate_mipmap) {
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }

        Texture(const Texture&) = delete;
        Texture(Texture&& source) noexcept;

        ~Texture();
    };
}