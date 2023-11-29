//
// Created by gomkyung2 on 11/26/23.
//

#include <OGLWrapper/Texture.hpp>

#include <algorithm>

namespace {
    template <typename T>
    constexpr bool isOneOf(const T &value, std::initializer_list<T> values) {
        const auto *const it = std::ranges::find(values, value);
        return it != values.end();
    }
}

bool OGLWrapper::Texture::isValidTarget(GLenum target) {
    return isOneOf(target,
        std::initializer_list<GLenum> { GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_1D_ARRAY,
            GL_TEXTURE_2D_ARRAY, GL_TEXTURE_RECTANGLE, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY,
            GL_TEXTURE_BUFFER, GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_2D_MULTISAMPLE_ARRAY }
    );
}

bool OGLWrapper::Texture::isValidInternalFormat(GLint internal_format) {
    // See https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
    // TODO: add more formats.

    return isOneOf(internal_format,
        { GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL, GL_RED, GL_RG, GL_RGB, GL_RGBA }
    );
}

bool OGLWrapper::Texture::isValidFormat(GLenum format) {
    return isOneOf(static_cast<GLint>(format),
        { GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA }
    );
}

bool OGLWrapper::Texture::isValidType(GLenum type) {
    return isOneOf(static_cast<GLint>(type),
        { GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT }
    );
}

OGLWrapper::Texture::Texture(Texture&& source) noexcept : target { source.target }, handle { source.handle } {
    source.handle = 0;
}

OGLWrapper::Texture::~Texture() {
    if (handle != 0) {
        glDeleteTextures(1, &handle);
    }
}
