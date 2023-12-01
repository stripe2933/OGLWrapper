//
// Created by gomkyung2 on 11/26/23.
//

#pragma once

#include <string>
#include <filesystem>

#include <GL/gl3w.h>

#include "strict_mode.hpp"
#include "gl_constraints.hpp"

namespace OGLWrapper {
    namespace details {
        std::string read_file(const std::filesystem::path &path);
    }

    template <GLenum ShaderType> requires (GLConstraints::isShaderType<ShaderType>())
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
        static constexpr GLenum shader_type = ShaderType;

        GLuint handle;

        template <bool CheckCompileStatus = OGLWRAPPER_STRICT_MODE>
        explicit Shader(const std::filesystem::path &source_path)
                : handle { glCreateShader(ShaderType) }
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

        Shader &operator=(const Shader&) = delete;
        Shader &operator=(Shader&& source) noexcept {
            glDeleteShader(handle);
            handle = std::exchange(source.handle, 0);
            return *this;
        }

        ~Shader() {
            if (handle != 0) {
                glDeleteShader(handle);
            }
        }
    };

    using VertexShader = Shader<GL_VERTEX_SHADER>;
    using FragmentShader = Shader<GL_FRAGMENT_SHADER>;
    using GeometryShader = Shader<GL_GEOMETRY_SHADER>;
    using TessControlShader = Shader<GL_TESS_CONTROL_SHADER>;
    using TessEvaluationShader = Shader<GL_TESS_EVALUATION_SHADER>;
    using ComputeShader = Shader<GL_COMPUTE_SHADER>;
}