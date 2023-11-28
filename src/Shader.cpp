//
// Created by gomkyung2 on 11/26/23.
//

#include <OGLWrapper/Shader.hpp>

#include <fstream>
#include <sstream>

std::string OGLWrapper::details::read_file(const std::filesystem::path &path) {
    std::ifstream file { path };
    if (!file) {
        throw std::runtime_error { "Failed to open file" };
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}