//
// Created by gomkyung2 on 11/28/23.
//

#include <OGLWrapper/Helper/OrthographicProjection.hpp>

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 OGLWrapper::Helper::OrthographicProjection::getMatrix(float left, float right, float bottom, float top) const {
    return glm::ortho(left, right, bottom, top, near, far);
}
