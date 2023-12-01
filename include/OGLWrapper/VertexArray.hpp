//
// Created by gomkyung2 on 11/27/23.
//

#pragma once

#include <GL/gl3w.h>

namespace OGLWrapper{
    struct VertexArray{
        GLuint handle;

        VertexArray() {
            glGenVertexArrays(1, &handle);
        }
        VertexArray(const VertexArray&) = delete; // VertexArray could not be copied.
        VertexArray(VertexArray &&source) noexcept : handle { source.handle } {
            source.handle = 0;
        }

        ~VertexArray() {
            glDeleteVertexArrays(1, &handle);
        }

		void bind() const{
			glBindVertexArray(handle);
		};
    };
}