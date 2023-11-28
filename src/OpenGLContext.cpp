//
// Created by gomkyung2 on 11/28/23.
//

#include <OGLWrapper/OpenGLContext.hpp>

#include <stdexcept>

#include <GL/gl3w.h>

OGLWrapper::OpenGLContext::OpenGLContext() {
    if (gl3wInit()){
        throw std::runtime_error { "Failed to initialize gl3w" };
    }
}