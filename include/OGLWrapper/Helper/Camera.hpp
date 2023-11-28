//
// Created by gomkyung2 on 11/28/23.
//

#pragma once

#include <type_traits>

#include <glm/gtc/matrix_transform.hpp>

#include "PerspectiveProjection.hpp"
#include "OrthographicProjection.hpp"

namespace OGLWrapper::Helper{
    namespace details {
        template <typename T, typename... Ts>
        concept one_of = std::disjunction_v<std::is_same<T, Ts>...>;
    }

    template <details::one_of<PerspectiveProjection, OrthographicProjection> Projection>
    struct Camera{
        constexpr glm::vec3 world_up { 0.f, 1.f, 0.f };

        glm::vec3 position;
        glm::vec3 target;
        Projection projection;

        constexpr glm::mat4 getViewMatrix() const noexcept {
            return lookAt(position, target, world_up);
        }

        constexpr glm::vec3 getTargetDisplacement() const noexcept {
            return target - position;
        }

        constexpr float getTargetDistance() const noexcept {
            return length(getTargetDisplacement());
        }

        constexpr glm::vec3 getFront() const noexcept {
            return normalize(getTargetDisplacement());
        }

        constexpr glm::vec3 getRight() const noexcept {
            return normalize(cross(getFront(), world_up));
        }

        constexpr glm::vec3 getUp() const noexcept {
            return normalize(cross(getRight(), getFront()));
        }
    };
}