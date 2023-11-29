//
// Created by gomkyung2 on 11/26/23.
//

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <eventpp/callbacklist.h>

#include "GlfwEvent.hpp"

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

        eventpp::CallbackList<void(GlfwEvent&, glm::ivec2 /* size */), GlfwEvent::policy> size_callback;
        eventpp::CallbackList<void(GlfwEvent&, glm::ivec2 /* size */), GlfwEvent::policy> framebuffer_size_callback;
        eventpp::CallbackList<void(GlfwEvent&, glm::vec2 /* scale */), GlfwEvent::policy> content_scale_callback;
        eventpp::CallbackList<void(GlfwEvent&, int /* key */, int /* scancode */, int /* action */, int /* mods */), GlfwEvent::policy> key_callback;
        eventpp::CallbackList<void(GlfwEvent&, unsigned int /* codepoint */), GlfwEvent::policy> char_callback;
        eventpp::CallbackList<void(GlfwEvent&, glm::dvec2 /* position */), GlfwEvent::policy> cursor_pos_callback;
        eventpp::CallbackList<void(GlfwEvent&, int /* entered */), GlfwEvent::policy> cursor_enter_callback;
        eventpp::CallbackList<void(GlfwEvent&, int /* button */, int /* action */, int /* mods */), GlfwEvent::policy> mouse_button_callback;
        eventpp::CallbackList<void(GlfwEvent&, glm::dvec2 /* offset */), GlfwEvent::policy> scroll_callback;
        eventpp::CallbackList<void(GlfwEvent&, int /* count */, const char ** /* paths */), GlfwEvent::policy> drop_callback;

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