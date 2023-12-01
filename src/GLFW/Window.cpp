//
// Created by gomkyung2 on 11/26/23.
//

#include <OGLWrapper/GLFW/Window.hpp>

#include <stdexcept>

void OGLWrapper::GLFW::WindowHint::setup() const {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_version_minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, opengl_forward_compat);
    glfwWindowHint(GLFW_OPENGL_PROFILE, opengl_profile);
}

OGLWrapper::GLFW::Window::Window(int width, int height, const char* title, const WindowHint& hint) {
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
        Window *base = static_cast<Window*>(glfwGetWindowUserPointer(window));

        GLFW::EventArg event{};
        base->size_callback(event, { width, height });
    });
    glfwSetFramebufferSizeCallback(base, [](GLFWwindow *window, int width, int height) {
        Window *base = static_cast<Window*>(glfwGetWindowUserPointer(window));

        GLFW::EventArg event{};
        base->framebuffer_size_callback(event, { width, height });
    });
    glfwSetWindowContentScaleCallback(base, [](GLFWwindow *window, float xscale, float yscale) {
        Window *base = static_cast<Window*>(glfwGetWindowUserPointer(window));

        GLFW::EventArg event{};
        base->content_scale_callback(event, { xscale, yscale });
    });
    glfwSetKeyCallback(base, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        Window *base = static_cast<Window*>(glfwGetWindowUserPointer(window));

        GLFW::EventArg event{};
        base->key_callback(event, key, scancode, action, mods);
    });
    glfwSetCharCallback(base, [](GLFWwindow *window, unsigned int codepoint) {
        Window *base = static_cast<Window*>(glfwGetWindowUserPointer(window));

        GLFW::EventArg event{};
        base->char_callback(event, codepoint);
    });
    glfwSetCursorPosCallback(base, [](GLFWwindow *window, double xpos, double ypos) {
        Window *base = static_cast<Window*>(glfwGetWindowUserPointer(window));

        GLFW::EventArg event{};
        base->cursor_pos_callback(event, { xpos, ypos });
    });
    glfwSetCursorEnterCallback(base, [](GLFWwindow *window, int entered) {
        Window *base = static_cast<Window*>(glfwGetWindowUserPointer(window));

        GLFW::EventArg event{};
        base->cursor_enter_callback(event, entered);
    });
    glfwSetMouseButtonCallback(base, [](GLFWwindow *window, int button, int action, int mods) {
        Window *base = static_cast<Window*>(glfwGetWindowUserPointer(window));

        GLFW::EventArg event{};
        base->mouse_button_callback(event, button, action, mods);
    });
    glfwSetScrollCallback(base, [](GLFWwindow *window, double xoffset, double yoffset) {
        Window *base = static_cast<Window*>(glfwGetWindowUserPointer(window));

        GLFW::EventArg event{};
        base->scroll_callback(event, { xoffset, yoffset });
    });
    glfwSetDropCallback(base, [](GLFWwindow *window, int count, const char **paths) {
        Window *base = static_cast<Window*>(glfwGetWindowUserPointer(window));

        GLFW::EventArg event{};
        base->drop_callback(event, count, paths);
    });
}

OGLWrapper::GLFW::Window::~Window() {
    glfwDestroyWindow(base);
    glfwTerminate();
}

glm::ivec2 OGLWrapper::GLFW::Window::getSize() const {
    glm::ivec2 size;
    glfwGetWindowSize(base, &size.x, &size.y);
    return size;
}

glm::ivec2 OGLWrapper::GLFW::Window::getFramebufferSize() const {
    glm::ivec2 size;
    glfwGetFramebufferSize(base, &size.x, &size.y);
    return size;
}

float OGLWrapper::GLFW::Window::getFramebufferAspectRatio() const {
    glm::ivec2 framebuffer_size = getFramebufferSize();
    return static_cast<float>(framebuffer_size.x) / static_cast<float>(framebuffer_size.y);
}

glm::vec2 OGLWrapper::GLFW::Window::getContentScale() const {
    glm::vec2 scale;
    glfwGetWindowContentScale(base, &scale.x, &scale.y);
    return scale;
}

glm::dvec2 OGLWrapper::GLFW::Window::getCursorPosition() const {
    glm::dvec2 position;
    glfwGetCursorPos(base, &position.x, &position.y);
    return position;
}

void OGLWrapper::GLFW::Window::run() {
    float elapsed_time = 0.f;
    while (!glfwWindowShouldClose(base)) {
        const auto time_delta = static_cast<float>(glfwGetTime()) - elapsed_time;
        elapsed_time += time_delta;

        onRenderLoop(time_delta);

        glfwPollEvents();
        glfwSwapBuffers(base);
    }
}
