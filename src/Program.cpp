//
// Created by gomkyung2 on 11/26/23.
//

#include <OGLWrapper/Program.hpp>

#include <stdexcept>

void OGLWrapper::Program::checkLinkStatus() const {
    GLint success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);

    if (!success) {
        constexpr GLsizei max_info_log_length = 512;
        std::string info_log(max_info_log_length, ' ');
        glGetProgramInfoLog(handle, max_info_log_length, nullptr, info_log.data());

        throw std::runtime_error { info_log };
    }
}

OGLWrapper::Program::Program(Program&& source) noexcept : handle { source.handle } {
    source.handle = 0;
}

GLint OGLWrapper::Program::getUniformLocation(const char* name) const {
    // Find uniform in cached locations.
    const auto it = uniform_locations.find(name);
    if (it != uniform_locations.end()) {
        return it->second;
    }

    // Uniform not found in cache. Query OpenGL.
    const GLint location = glGetUniformLocation(handle, name);
    if (location == -1) {
        throw std::runtime_error { "Uniform not found." };
    }

    // Insert the found location to the cache.
    uniform_locations.insert(it, { name, location });

    return location;
}

OGLWrapper::Program::~Program() {
    if (handle != 0) {
        glDeleteProgram(handle);
    }
}

void OGLWrapper::Program::use() const {
    glUseProgram(handle);
}
