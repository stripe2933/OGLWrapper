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

        void setup(GLenum target) const {
            glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
            glTexParameteri(target, GL_TEXTURE_WRAP_R, wrap_r);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter);
        }
    };

    class Texture {
        static bool isValidTarget(GLenum target);

    public:
        GLenum target;
        GLuint handle;

        template <bool CheckTarget = OGLWRAPPER_STRICT_MODE>
        explicit Texture(GLenum target) : target { target } {
            if constexpr (CheckTarget) {
                if (!isValidTarget(target)) {
                    throw std::invalid_argument { "Invalid target" };
                }
            }

            glGenTextures(1, &handle);
        }
        Texture(const Texture&) = delete;
        Texture(Texture&& source) noexcept;

        ~Texture();
    };
}