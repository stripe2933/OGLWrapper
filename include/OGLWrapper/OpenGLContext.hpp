//
// Created by gomkyung2 on 11/28/23.
//

#pragma once

namespace OGLWrapper{
    /**
     * Your every OpenGL app should initialize the gl3w context before any OpenGL operation.
     * If your application is encapsulated in class, declare this struct in the top of the member field
     * will initialize the gl3w context at class construction time.
     *
     * @example
     * @code
     * class MyOpenGLApp : OGLWrapper::GlfwWindow {
     *     // Initialized right after GlfwWindow construction, before any fields declared in this class.
     *     OGLWrapper::OpenGLContext context{};
     *
     *     // Any other fields...
     *
     *     MyOpenGLApp();
     *     ~MyOpenGLApp();
     *
     *     // Any other methods...
     * };
     * @endcode
     **/
    struct OpenGLContext{
        OpenGLContext();
        OpenGLContext(const OpenGLContext&) = delete;
        OpenGLContext(OpenGLContext&&) = delete;
    };
}