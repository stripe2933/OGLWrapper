//
// Created by gomkyung2 on 11/26/23.
//

#include <OGLWrapper/Helper/Image.hpp>

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLint Image::mapChannelToInternalFormat(int channels) {
    switch (channels) {
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: throw std::invalid_argument { "Invalid channel count" };
    }
}

GLenum Image::mapChannelToFormat(int channels) {
    switch (channels) {
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: throw std::invalid_argument { "Invalid channel count" };
    }
}

Image::Image(const char * source, bool flip) {
    stbi_set_flip_vertically_on_load(flip);
    data = stbi_load(source, &width, &height, &channels, 0);

    if (!data) {
        throw std::runtime_error { stbi_failure_reason() };
    }
}

Image::Image(Image&& source) noexcept
        : width { source.width }, height { source.height }, channels { source.channels }, data { source.data }
{
    source.data = nullptr;
}

OGLWrapper::Texture Image::toTexture(const OGLWrapper::TextureParamter &parameter, bool generate_mipmap) {
    return OGLWrapper::Texture { mapChannelToInternalFormat(channels), width, height,
                             mapChannelToFormat(channels), GL_UNSIGNED_BYTE, data, parameter, generate_mipmap };

}

Image::~Image() {
    if (data != nullptr) {
        stbi_image_free(data);
    }
}
