//
// Created by gomkyung2 on 11/28/23.
//

#pragma once

#include <glm/ext/matrix_float4x4.hpp>

namespace OGLWrapper::Helper {
    struct OrthographicProjection {
        float near;
        float far;

        glm::mat4 getMatrix(float left, float right, float bottom, float top) const;
    };
}