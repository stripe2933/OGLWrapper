//
// Created by gomkyung2 on 11/26/23.
//

#pragma once

#include <stdexcept>

#include <GL/gl3w.h>

#include "strict_mode.hpp"

namespace OGLWrapper{
    struct TextureParameter {
        GLint wrap_s = GL_REPEAT;
        GLint wrap_t = GL_REPEAT;
        GLint wrap_r = GL_REPEAT;
        GLint min_filter = GL_LINEAR_MIPMAP_LINEAR;
        GLint mag_filter = GL_LINEAR;

        void setup() const {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrap_r);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
        }
    };

    class Texture {
        static bool isValidTarget(GLenum target);
        static bool isValidInternalFormat(GLint internal_format);
        static bool isValidFormat(GLenum format);
        static bool isValidType(GLenum type);

    public:
        GLenum target;
        GLuint handle;

        template <bool CheckFormat = OGLWRAPPER_STRICT_MODE>
        Texture(GLenum target, GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type,
                const void *data, const TextureParameter &parameter, bool generate_mipmap = true) : target { target }
        {
            if constexpr (CheckFormat) {
                if (!isValidTarget(target)) {
                    throw std::invalid_argument { "Invalid target" };
                }
                if (!isValidInternalFormat(internal_format)) {
                    throw std::invalid_argument { "Invalid internal format" };
                }
                if (!isValidFormat(format)) {
                    throw std::invalid_argument { "Invalid format" };
                }
                if (!isValidType(type)) {
                    throw std::invalid_argument { "Invalid type" };
                }
            }

            glGenTextures(1, &handle);

            glBindTexture(target, handle);
            parameter.setup();

            glTexImage2D(target, 0, internal_format, width, height, 0, format, type, data);
            if (generate_mipmap) {
                glGenerateMipmap(target);
            }
        }

        Texture(const Texture&) = delete;
        Texture(Texture&& source) noexcept;

        ~Texture();
    };
}