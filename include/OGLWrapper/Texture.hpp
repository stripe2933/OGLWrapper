//
// Created by gomkyung2 on 11/26/23.
//

#pragma once

#include <GL/gl3w.h>

#include "gl_constraints.hpp"

namespace OGLWrapper{
    template <GLenum Target> requires (GLConstraints::isTextureTarget<Target>())
    struct Texture {
        static constexpr GLenum target = Target;

        GLuint handle;

        explicit Texture() {
            glGenTextures(1, &handle);
        }
        Texture(const Texture&) = delete;
        Texture(Texture&& source) noexcept : handle { source.handle } {
            source.handle = 0;
        }

        Texture &operator=(const Texture&) = delete;
        Texture &operator=(Texture&& source) noexcept {
            glDeleteTextures(1, &handle);
            handle = std::exchange(source.handle, 0);
            return *this;
        }

        ~Texture() {
            if (handle != 0) {
                glDeleteTextures(1, &handle);
            }
        }

        void bind() const {
            glBindTexture(Target, handle);
        }
    };
}