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

OGLWrapper::Texture::Texture(Texture&& source) noexcept : target { source.target }, handle { source.handle } {
    source.handle = 0;
}

OGLWrapper::Texture::~Texture() {
    if (handle != 0) {
        glDeleteTextures(1, &handle);
    }
}
