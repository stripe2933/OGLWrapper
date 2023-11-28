//
// Created by gomkyung2 on 11/26/23.
//

#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

struct GlfwWindowHint {
    int context_version_major = 3;
    int context_version_minor = 3;
    int opengl_forward_compat = GLFW_TRUE;
    int opengl_profile = GLFW_OPENGL_CORE_PROFILE;

    void setup() const;
};

class GlfwWindow{
protected:
    GLFWwindow *base;

    virtual void onRenderLoop(float time_delta) = 0;

    virtual void onSizeCallback(glm::ivec2 size) { }
    virtual void onFramebufferSizeCallback(glm::ivec2 size) { }
    virtual void onContentScaleCallback(glm::vec2 scale) { }
    virtual void onKeyCallback(int key, int scancode, int action, int mods) { }
    virtual void onCharCallback(unsigned int codepoint) { }
    virtual void onCursorPosCallback(glm::dvec2 position) { }
    virtual void onCursorEnterCallback(int entered) { }
    virtual void onMouseButtonCallback(int button, int action, int mods) { }
    virtual void onScrollCallback(glm::dvec2 offset) { }
    virtual void onDropCallback(int count, const char **paths) { }

public:
    GlfwWindow(int width, int height, const char *title, const GlfwWindowHint &hint = {});

    virtual ~GlfwWindow();

    glm::ivec2 getSize() const;
    glm::ivec2 getFramebufferSize() const;
    glm::vec2 getContentScale() const;

    glm::dvec2 getCursorPosition() const;

    void run();
};