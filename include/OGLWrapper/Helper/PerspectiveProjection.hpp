//
// Created by gomkyung2 on 11/28/23.
//

#pragma once

#include <glm/ext/matrix_float4x4.hpp>

namespace OGLWrapper::Helper {
    struct PerspectiveProjection {
        float fov;
        float near;
        float far;

        glm::mat4 getMatrix(float aspect_ratio) const;
    };
}