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

bool OGLWrapper::Texture::isValidInternalFormat(GLint internal_format) {
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

OGLWrapper::Texture::Texture(Texture&& source) noexcept : handle { source.handle } {
    source.handle = 0;
}

OGLWrapper::Texture::~Texture() {
    if (handle != 0) {
        glDeleteTextures(1, &handle);
    }
}
