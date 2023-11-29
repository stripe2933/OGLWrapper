//
// Created by gomkyung2 on 11/29/23.
//

#pragma once

namespace OGLWrapper::GLFW{
    struct GlfwEvent{
        struct policy{
            static bool canContinueInvoking(GlfwEvent &event, auto &&...) noexcept {
                return !event.canceled;
            }
        };

        bool canceled = false;
    };
}