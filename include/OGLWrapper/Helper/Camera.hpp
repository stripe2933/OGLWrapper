//
// Created by gomkyung2 on 11/28/23.
//

#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace OGLWrapper::Helper::Camera{
    static glm::vec3 getPosition(const glm::mat4 &view) noexcept {
        return view[3];
    }

    static glm::vec3 getFront(const glm::mat4 &view) noexcept {
        return -view[2];
    }

    static glm::vec3 getRight(const glm::mat4 &view) noexcept {
        return view[0];
    }

    static glm::vec3 getUp(const glm::mat4 &view) noexcept {
        return view[1];
    }
}