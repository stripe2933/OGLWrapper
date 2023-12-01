//
// Created by gomkyung2 on 12/1/23.
//

#pragma once

#include <GL/gl3w.h>

#include "Texture.hpp"
#include "Renderbuffer.hpp"
#include "gl_constraints.hpp"
#include "strict_mode.hpp"

namespace OGLWrapper {
    template <GLenum Target> requires (GLConstraints::isFramebufferTarget<Target>())
    struct Framebuffer {
        static constexpr GLenum target = Target;

        GLuint handle;

        Framebuffer() {
            glGenFramebuffers(1, &handle);
        }
        Framebuffer(const Framebuffer&) = delete;
        Framebuffer(Framebuffer&& source) : handle { source.handle } {
            source.handle = 0;
        }

        Framebuffer &operator=(const Framebuffer&) = delete;
        Framebuffer &operator=(Framebuffer&& source) noexcept {
            glDeleteFramebuffers(1, &handle);
            handle = std::exchange(source.handle, 0);
            return *this;
        }

        ~Framebuffer() {
            if (handle != 0) {
                glDeleteFramebuffers(1, &handle);
            }
        }

        void bind() const {
            glBindFramebuffer(Target, handle);
        }

        template <bool CheckAttachment = OGLWRAPPER_STRICT_MODE>
        void attachTexture(GLenum attachment, const auto &texture /* TODO: add type check */) {
            if constexpr (CheckAttachment) {
                if (!GLConstraints::isAttachmentType(attachment)) {
                    throw std::invalid_argument { "Invalid attachment type." };
                }
            }

            bind();
            glFramebufferTexture2D(Target, attachment, texture.target, texture.handle, 0);
        }

        template <bool CheckAttachment = OGLWRAPPER_STRICT_MODE>
        void attachRenderbuffer(GLenum attachment, const Renderbuffer &renderbuffer) {
            if constexpr (CheckAttachment) {
                if (!GLConstraints::isAttachmentType(attachment)) {
                    throw std::invalid_argument { "Invalid attachment type." };
                }
            }

            bind();
            glFramebufferRenderbuffer(Target, attachment, GL_RENDERBUFFER, renderbuffer.handle);
        }

        bool isComplete() const {
            bind();
            return glCheckFramebufferStatus(Target) == GL_FRAMEBUFFER_COMPLETE;
        }

        static void restoreToDefault() {
            glBindFramebuffer(Target, 0);
        }
    };
}