//
// Created by gomkyung2 on 11/26/23.
//

#pragma once

#include <string>
#include <filesystem>

#include <GL/gl3w.h>

#include "strict_mode.hpp"

namespace OGLWrapper {
    namespace details {
        std::string read_file(const std::filesystem::path &path);
    }

    enum class ShaderType : GLenum {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
    };

    template <ShaderType ShaderT>
    class Shader final {
        void checkCompileStatus() const {
            GLint success;
            glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

            if (!success) {
                constexpr GLsizei max_info_log_length = 512;
                std::string info_log(max_info_log_length, ' ');
                glGetShaderInfoLog(handle, max_info_log_length, nullptr, info_log.data());

                throw std::runtime_error { info_log };
            }
        }

    public:
        static constexpr ShaderType shader_type = ShaderT;

        GLuint handle;

        template <bool CheckCompileStatus = STRICT_MODE>
        explicit Shader(const char *source) : handle { glCreateShader(static_cast<GLenum>(ShaderT)) } {
            glShaderSource(handle, 1, &source, nullptr);
            glCompileShader(handle);

            if constexpr (CheckCompileStatus) {
                checkCompileStatus();
            }
        }

        template <bool CheckCompileStatus = STRICT_MODE>
        explicit Shader(const std::filesystem::path &source_path)
                : handle { glCreateShader(static_cast<GLenum>(ShaderT)) }
        {
            const std::string shader_source = details::read_file(source_path);
            const char* const shader_source_c_str = shader_source.c_str();
            const GLint shader_source_length = shader_source.size();

            glShaderSource(handle, 1, &shader_source_c_str, &shader_source_length);
            glCompileShader(handle);

            if constexpr (CheckCompileStatus) {
                checkCompileStatus();
            }
        }

        Shader(const Shader&) = delete; // Shader could not be copied.
        Shader(Shader &&source) noexcept : handle { source.handle } {
            handle = 0;
        }

        ~Shader() {
            if (handle != 0) {
                glDeleteShader(handle);
            }
        }
    };

    using VertexShader = Shader<ShaderType::Vertex>;
    using FragmentShader = Shader<ShaderType::Fragment>;
    using GeometryShader = Shader<ShaderType::Geometry>;
}