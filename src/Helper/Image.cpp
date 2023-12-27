//
// Created by gomkyung2 on 11/26/23.
//

#include <OGLWrapper/Helper/Image.hpp>

#include <stdexcept>
#include <initializer_list>
#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLint OGLWrapper::Helper::Image::mapChannelToInternalFormat(int channels) {
    switch (channels) {
        case 1: return GL_RED;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: throw std::invalid_argument { "Invalid channel count" };
    }
}

GLenum OGLWrapper::Helper::Image::mapChannelToFormat(int channels) {
    switch (channels) {
        case 1: return GL_RED;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: throw std::invalid_argument { "Invalid channel count" };
    }
}

OGLWrapper::Helper::Image::Image(const char * source, bool flip) {
    stbi_set_flip_vertically_on_load(flip);
    data = stbi_load(source, &width, &height, &channels, 0);

    if (!data) {
        throw std::runtime_error { stbi_failure_reason() };
    }
}

OGLWrapper::Helper::Image::Image(Image&& source) noexcept
        : width { source.width }, height { source.height }, channels { source.channels }, data { source.data }
{
    source.data = nullptr;
}

OGLWrapper::Texture<GL_TEXTURE_CUBE_MAP> OGLWrapper::Helper::Image::toCubemap(const Image& right, const Image& left, const Image& top,
    const Image& bottom, const Image& front, const Image& back)
{
    OGLWrapper::Texture<GL_TEXTURE_CUBE_MAP> texture{};
    texture.bind();

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    []<typename... Images, std::size_t... Is>
        requires (sizeof...(Images) == sizeof...(Is))
    (std::index_sequence<Is...>, const Images &...image){
        ((glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + Is,
            0,
            mapChannelToInternalFormat(image.channels),
            image.width, image.height,
            0,
            mapChannelToFormat(image.channels),
            GL_UNSIGNED_BYTE,
            image.data)), ...);
    }(std::make_index_sequence<6>{}, right, left, top, bottom, front, back);

    return texture;
}

OGLWrapper::Helper::Image::~Image() {
    if (data != nullptr) {
        stbi_image_free(data);
    }
}
