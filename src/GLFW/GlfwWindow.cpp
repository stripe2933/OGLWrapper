//
// Created by gomkyung2 on 11/26/23.
//

#include <OGLWrapper/GLFW/GlfwWindow.hpp>

#include <stdexcept>

void GlfwWindowHint::setup() const {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_version_minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, opengl_forward_compat);
    glfwWindowHint(GLFW_OPENGL_PROFILE, opengl_profile);
}

GlfwWindow::GlfwWindow(int width, int height, const char* title, const GlfwWindowHint& hint) {
    if (!glfwInit()) {
        throw std::runtime_error { "Failed to initialize GLFW" };
    }
    hint.setup();

    base = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!base) {
        throw std::runtime_error { "Failed to create window" };
    }

    glfwMakeContextCurrent(base);

    glfwSetWindowUserPointer(base, this);

    glfwSetWindowSizeCallback(base, [](GLFWwindow *window, int width, int height) {
        GlfwWindow *base = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        base->onSizeCallback({ width, height });
    });
    glfwSetFramebufferSizeCallback(base, [](GLFWwindow *window, int width, int height) {
        GlfwWindow *base = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        base->onFramebufferSizeCallback({ width, height });
    });
    glfwSetWindowContentScaleCallback(base, [](GLFWwindow *window, float xscale, float yscale) {
        GlfwWindow *base = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        base->onContentScaleCallback({ xscale, yscale });
    });
    glfwSetKeyCallback(base, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        GlfwWindow *base = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        base->onKeyCallback(key, scancode, action, mods);
    });
    glfwSetCharCallback(base, [](GLFWwindow *window, unsigned int codepoint) {
        GlfwWindow *base = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        base->onCharCallback(codepoint);
    });
    glfwSetCursorPosCallback(base, [](GLFWwindow *window, double xpos, double ypos) {
        GlfwWindow *base = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        base->onCursorPosCallback({ xpos, ypos });
    });
    glfwSetCursorEnterCallback(base, [](GLFWwindow *window, int entered) {
        GlfwWindow *base = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        base->onCursorEnterCallback(entered);
    });
    glfwSetMouseButtonCallback(base, [](GLFWwindow *window, int button, int action, int mods) {
        GlfwWindow *base = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        base->onMouseButtonCallback(button, action, mods);
    });
    glfwSetScrollCallback(base, [](GLFWwindow *window, double xoffset, double yoffset) {
        GlfwWindow *base = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        base->onScrollCallback({ xoffset, yoffset });
    });
    glfwSetDropCallback(base, [](GLFWwindow *window, int count, const char **paths) {
        GlfwWindow *base = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        base->onDropCallback(count, paths);
    });
}

GlfwWindow::~GlfwWindow() {
    glfwDestroyWindow(base);
    glfwTerminate();
}

glm::ivec2 GlfwWindow::getSize() const {
    glm::ivec2 size;
    glfwGetWindowSize(base, &size.x, &size.y);
    return size;
}

glm::ivec2 GlfwWindow::getFramebufferSize() const {
    glm::ivec2 size;
    glfwGetFramebufferSize(base, &size.x, &size.y);
    return size;
}

glm::vec2 GlfwWindow::getContentScale() const {
    glm::vec2 scale;
    glfwGetWindowContentScale(base, &scale.x, &scale.y);
    return scale;
}

glm::dvec2 GlfwWindow::getCursorPosition() const {
    glm::dvec2 position;
    glfwGetCursorPos(base, &position.x, &position.y);
    return position;
}

void GlfwWindow::run() {
    float elapsed_time = 0.f;
    while (!glfwWindowShouldClose(base)) {
        const auto time_delta = static_cast<float>(glfwGetTime()) - elapsed_time;
        elapsed_time += time_delta;

        onRenderLoop(time_delta);

        glfwPollEvents();
        glfwSwapBuffers(base);
    }
}
