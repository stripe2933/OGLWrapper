//
// Created by gomkyung2 on 11/26/23.
//

#include <OGLWrapper/Program.hpp>

#include <stdexcept>

void OGLWrapper::Program::checkLinkStatus(GLuint handle) {
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

void OGLWrapper::Program::pendUniforms(std::function<void()> command) const {
    pending_uniform_commands.emplace(std::move(command));
}

void OGLWrapper::Program::setUniformBlockBinding(const char* name, GLuint binding_point) const {
    const GLuint index = glGetUniformBlockIndex(handle, name);
    glUniformBlockBinding(handle, index, binding_point);
}

OGLWrapper::ProgramBuilder::ProgramBuilder() {
    program = glCreateProgram();
}

OGLWrapper::ProgramBuilder::~ProgramBuilder() {
    if (program != 0) {
        glDeleteProgram(program);
    }
}

OGLWrapper::ProgramBuilder& OGLWrapper::ProgramBuilder::setFeedbackVaryings(std::span<const char* const> varyings,
    GLenum buffer_mode) {
    // TODO: check if buffer mode is valid.
    glTransformFeedbackVaryings(program, varyings.size(), varyings.data(), buffer_mode);
    return *this;
}

OGLWrapper::Program& OGLWrapper::Program::operator=(Program&& source) noexcept {
    glDeleteProgram(handle);
    handle = std::exchange(source.handle, 0);
    uniform_locations = std::move(source.uniform_locations);
    return *this;
}

OGLWrapper::Program::~Program() {
    if (handle != 0) {
        glDeleteProgram(handle);
    }
}

void OGLWrapper::Program::use() const {
    glUseProgram(handle);

    while (!pending_uniform_commands.empty()) {
        pending_uniform_commands.front()();
        pending_uniform_commands.pop();
    }
}
