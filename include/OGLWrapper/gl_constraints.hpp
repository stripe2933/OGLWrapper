//
// Created by gomkyung2 on 12/1/23.
//

#pragma once

#include <algorithm>
#include <initializer_list>

#include <GL/gl3w.h>

namespace OGLWrapper::GLConstraints {
    namespace details {
        template <typename T>
        constexpr bool isOneOf(const T&value, std::initializer_list<T> values) {
            const auto* const it = std::ranges::find(values, value);
            return it != values.end();
        }
    }

    template <GLenum Target>
    consteval bool isTextureTarget() noexcept {
        return details::isOneOf<GLenum>(Target, {
                                            GL_TEXTURE_1D,
                                            GL_TEXTURE_2D,
                                            GL_TEXTURE_3D,
                                            GL_TEXTURE_1D_ARRAY,
                                            GL_TEXTURE_2D_ARRAY,
                                            GL_TEXTURE_RECTANGLE,
                                            GL_TEXTURE_CUBE_MAP,
                                            GL_TEXTURE_CUBE_MAP_ARRAY,
                                            GL_TEXTURE_BUFFER,
                                            GL_TEXTURE_2D_MULTISAMPLE,
                                            GL_TEXTURE_2D_MULTISAMPLE_ARRAY
                                        });
    }

    template <GLenum Target>
    consteval bool isFramebufferTarget() noexcept {
        return details::isOneOf<GLenum>(Target,
                                        {
                                            GL_FRAMEBUFFER,
                                            GL_DRAW_FRAMEBUFFER,
                                            GL_READ_FRAMEBUFFER
                                        });
    }

    template <GLenum Target>
    consteval bool isBufferTarget() noexcept {
        return details::isOneOf<GLenum>(Target,
                                        {
                                            GL_ARRAY_BUFFER,
                                            GL_ATOMIC_COUNTER_BUFFER,
                                            GL_COPY_READ_BUFFER,
                                            GL_COPY_WRITE_BUFFER,
                                            GL_DISPATCH_INDIRECT_BUFFER,
                                            GL_DRAW_INDIRECT_BUFFER,
                                            GL_ELEMENT_ARRAY_BUFFER,
                                            GL_PIXEL_PACK_BUFFER,
                                            GL_PIXEL_UNPACK_BUFFER,
                                            GL_QUERY_BUFFER,
                                            GL_SHADER_STORAGE_BUFFER,
                                            GL_TEXTURE_BUFFER,
                                            GL_TRANSFORM_FEEDBACK_BUFFER,
                                            GL_UNIFORM_BUFFER
                                        });
    }

    constexpr bool isBufferUsage(GLenum usage) noexcept {
        return details::isOneOf<GLenum>(usage,
                                        {
                                            GL_STREAM_DRAW,
                                            GL_STREAM_READ,
                                            GL_STREAM_COPY,
                                            GL_STATIC_DRAW,
                                            GL_STATIC_READ,
                                            GL_STATIC_COPY,
                                            GL_DYNAMIC_DRAW,
                                            GL_DYNAMIC_READ,
                                            GL_DYNAMIC_COPY
                                        });
    }

    template <GLenum Type>
    consteval bool isShaderType() noexcept {
        return details::isOneOf<GLenum>(Type,
                                        {
                                            GL_VERTEX_SHADER,
                                            GL_TESS_CONTROL_SHADER,
                                            GL_TESS_EVALUATION_SHADER,
                                            GL_GEOMETRY_SHADER,
                                            GL_FRAGMENT_SHADER,
                                            GL_COMPUTE_SHADER
                                        });
    }

    constexpr bool isAttachmentType(GLenum type) noexcept {
        return details::isOneOf<GLenum>(type,
                                        {
                                            GL_COLOR_ATTACHMENT0,
                                            GL_COLOR_ATTACHMENT1,
                                            GL_COLOR_ATTACHMENT2,
                                            GL_COLOR_ATTACHMENT3,
                                            GL_COLOR_ATTACHMENT4,
                                            GL_COLOR_ATTACHMENT5,
                                            GL_COLOR_ATTACHMENT6,
                                            GL_COLOR_ATTACHMENT7,
                                            GL_COLOR_ATTACHMENT8,
                                            GL_COLOR_ATTACHMENT9,
                                            GL_COLOR_ATTACHMENT10,
                                            GL_COLOR_ATTACHMENT11,
                                            GL_COLOR_ATTACHMENT12,
                                            GL_COLOR_ATTACHMENT13,
                                            GL_COLOR_ATTACHMENT14,
                                            GL_COLOR_ATTACHMENT15,
                                            GL_DEPTH_ATTACHMENT,
                                            GL_STENCIL_ATTACHMENT,
                                            GL_DEPTH_STENCIL_ATTACHMENT
                                        });
    }

    constexpr bool isDrawMode(GLenum mode) noexcept {
        return details::isOneOf<GLenum>(mode, {
                                            GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY,
                                            GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES,
                                            GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY, GL_PATCHES
                                        });
    }
}
