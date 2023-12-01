//
// Created by gomkyung2 on 11/26/23.
//

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include <GL/gl3w.h>

#include "strict_mode.hpp"

namespace OGLWrapper {
    namespace details {
        struct StringHash {
            using hash_type = std::hash<std::string_view>;

            // don't forget the next line
            // this is what causes the lookup methods of unordered containers
            // to select template keys as opposed to constructing the defined key type
            using is_transparent = void;

            constexpr std::size_t operator()(const char* str) const noexcept { return hash_type{}(str); }
            constexpr std::size_t operator()(std::string_view str) const noexcept { return hash_type{}(str); }
            constexpr std::size_t operator()(std::string const& str) const noexcept { return hash_type{}(str); }
        };
    }

    class Program final {
        mutable std::unordered_map<std::string, GLint, details::StringHash, std::equal_to<>> uniform_locations;

        void checkLinkStatus() const;

    public:
        GLuint handle;

        // TODO: add constraints for Shaders.
        template <bool CheckLinkStatus = OGLWRAPPER_STRICT_MODE, typename... Shaders>
        explicit Program(Shaders &&...shaders)
                : handle { glCreateProgram() }
        {
            (glAttachShader(handle, shaders.handle), ...);
            glLinkProgram(handle);

            if constexpr (CheckLinkStatus) {
                checkLinkStatus();
            }
        }
        Program(const Program&) = delete; // Program could not be copied.
        Program(Program &&source) noexcept;

        ~Program();

        void use() const;
        GLint getUniformLocation(const char *name) const;
    };
}