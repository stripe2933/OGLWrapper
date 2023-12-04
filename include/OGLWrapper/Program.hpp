//
// Created by gomkyung2 on 11/26/23.
//

#pragma once

#include <queue>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

#include <GL/gl3w.h>

#include "strict_mode.hpp"

namespace OGLWrapper {
    namespace details {
        struct string_hasher {
            using hash_type = std::hash<std::string_view>;

            using is_transparent = void;

            constexpr std::size_t operator()(const char* str) const noexcept { return hash_type{}(str); }
            constexpr std::size_t operator()(std::string_view str) const noexcept { return hash_type{}(str); }
            constexpr std::size_t operator()(std::string const& str) const noexcept { return hash_type{}(str); }
        };
    }

    class Program final {
        mutable std::unordered_map<std::string, GLint, details::string_hasher, std::equal_to<>> uniform_locations;
        mutable std::queue<std::function<void()>> pending_uniform_commands;

        void checkLinkStatus() const;

    public:
        GLuint handle;

        // TODO: add constraints for Shaders.
        template <bool CheckLinkStatus = OGLWRAPPER_STRICT_MODE, typename... Shaders>
        explicit Program(Shaders &&...shaders) : handle { glCreateProgram() } {
            (glAttachShader(handle, shaders.handle), ...);
            glLinkProgram(handle);

            if constexpr (CheckLinkStatus) {
                checkLinkStatus();
            }
        }
        Program(const Program&) = delete; // Program could not be copied.
        Program(Program &&source) noexcept;

        Program &operator=(const Program&) = delete;
        Program &operator=(Program&& source) noexcept {
            glDeleteProgram(handle);
            handle = std::exchange(source.handle, 0);
            uniform_locations = std::move(source.uniform_locations);
            return *this;
        }

        ~Program();

        void use() const;
        GLint getUniformLocation(const char *name) const;
        void pendUniforms(std::function<void()> command) const;
        void setUniformBlockBinding(const char *name, GLuint binding_point) const;

        template <typename... Programs>
        static void setUniformBlockBindings(const char *name, GLuint binding_point, Programs &...programs);
    };

    template <typename... Programs>
    void Program::setUniformBlockBindings(const char* name, GLuint binding_point, Programs&... programs) {
        (programs.setUniformBlockBinding(name, binding_point), ...);
    }
}
