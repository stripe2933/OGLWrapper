//
// Created by gomkyung2 on 11/28/23.
//

#include <OGLWrapper/Helper/PerspectiveProjection.hpp>

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 OGLWrapper::Helper::PerspectiveProjection::getMatrix(float aspect_ratio) const {
    return glm::perspective(fov, aspect_ratio, near, far);
}
