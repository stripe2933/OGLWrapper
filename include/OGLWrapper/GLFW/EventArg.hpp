//
// Created by gomkyung2 on 11/29/23.
//

#pragma once

namespace OGLWrapper::GLFW{
    struct EventArg{
        struct policy{
            static bool canContinueInvoking(EventArg &arg, auto &&...) noexcept {
                return !arg.canceled;
            }
        };

        bool canceled = false;
    };
}