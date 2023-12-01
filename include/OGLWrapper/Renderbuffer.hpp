//
// Created by gomkyung2 on 12/2/23.
//

#pragma once

#include <GL/gl3w.h>

namespace OGLWrapper{
    struct Renderbuffer{
        GLuint handle;

        Renderbuffer() {
            glGenRenderbuffers(1, &handle);
        }
        Renderbuffer(const Renderbuffer&) = delete;
        Renderbuffer(Renderbuffer&& source) : handle { source.handle } {
            source.handle = 0;
        }

        Renderbuffer &operator=(const Renderbuffer&) = delete;
        Renderbuffer &operator=(Renderbuffer&& source) noexcept {
            glDeleteRenderbuffers(1, &handle);
            handle = std::exchange(source.handle, 0);
            return *this;
        }

        ~Renderbuffer() {
            if (handle != 0) {
                glDeleteRenderbuffers(1, &handle);
            }
        }

        void bind() const{
            glBindRenderbuffer(GL_RENDERBUFFER, handle);
        }
    };
}