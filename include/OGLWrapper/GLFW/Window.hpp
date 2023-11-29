//
// Created by gomkyung2 on 11/26/23.
//

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <eventpp/callbacklist.h>

#include "EventArg.hpp"

namespace OGLWrapper::GLFW {
    struct WindowHint {
        int context_version_major = 3;
        int context_version_minor = 3;
        int opengl_forward_compat = GLFW_TRUE;
        int opengl_profile = GLFW_OPENGL_CORE_PROFILE;

        void setup() const;
    };

    class Window{
    protected:
        GLFWwindow *base;

        eventpp::CallbackList<void(EventArg&, glm::ivec2 /* size */), EventArg::policy> size_callback;
        eventpp::CallbackList<void(EventArg&, glm::ivec2 /* size */), EventArg::policy> framebuffer_size_callback;
        eventpp::CallbackList<void(EventArg&, glm::vec2 /* scale */), EventArg::policy> content_scale_callback;
        eventpp::CallbackList<void(EventArg&, int /* key */, int /* scancode */, int /* action */, int /* mods */), EventArg::policy> key_callback;
        eventpp::CallbackList<void(EventArg&, unsigned int /* codepoint */), EventArg::policy> char_callback;
        eventpp::CallbackList<void(EventArg&, glm::dvec2 /* position */), EventArg::policy> cursor_pos_callback;
        eventpp::CallbackList<void(EventArg&, int /* entered */), EventArg::policy> cursor_enter_callback;
        eventpp::CallbackList<void(EventArg&, int /* button */, int /* action */, int /* mods */), EventArg::policy> mouse_button_callback;
        eventpp::CallbackList<void(EventArg&, glm::dvec2 /* offset */), EventArg::policy> scroll_callback;
        eventpp::CallbackList<void(EventArg&, int /* count */, const char ** /* paths */), EventArg::policy> drop_callback;

        virtual void onRenderLoop(float time_delta) = 0;

    public:
        Window(int width, int height, const char *title, const WindowHint &hint);
        virtual ~Window();

        glm::ivec2 getSize() const;
        glm::ivec2 getFramebufferSize() const;
        glm::vec2 getContentScale() const;

        glm::dvec2 getCursorPosition() const;

        void run();
    };
}