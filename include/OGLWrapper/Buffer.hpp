//
// Created by gomkyung2 on 11/27/23.
//

#pragma once

#include <span>
#include <stdexcept>

#include <GL/gl3w.h>

#include "strict_mode.hpp"
#include "gl_constraints.hpp"

namespace OGLWrapper{
    template <GLenum BufferTarget, typename T> requires (GLConstraints::isBufferTarget<BufferTarget>())
    struct BufferBase {
        GLenum usage;

		template <bool CheckUsage = OGLWRAPPER_STRICT_MODE>
        BufferBase(GLenum usage) noexcept(!CheckUsage) : usage { usage } {
			if constexpr (CheckUsage) {
				if (!GLConstraints::isBufferUsage(usage)){
					throw std::invalid_argument { "Invalid buffer usage." };
				}
			}
		}

        virtual void store(std::span<const T> data) = 0;
        virtual ~BufferBase() = default;
    };

    template <GLenum BufferTarget, typename T>
    struct Buffer;

    template <GLenum BufferTarget, typename T>
    class SubBuffer final : BufferBase<BufferTarget, T> {
        std::size_t size;
        std::ptrdiff_t offset;

        template <bool CheckOverflow = OGLWRAPPER_STRICT_MODE>
        explicit SubBuffer(const Buffer<BufferTarget, T> &buffer, std::size_t size, std::ptrdiff_t offset) noexcept(!CheckOverflow)
                : BufferBase<BufferTarget, T> { buffer.usage }, buffer { buffer }, size { size }, offset { offset }
        {
            if constexpr (CheckOverflow) {
                if (offset + size > buffer.capacity) {
                    throw std::out_of_range { "SubBuffer should be within the range of the buffer." };
                }
            }
        }

        friend struct Buffer<BufferTarget, T>;

    public:
        const Buffer<BufferTarget, T> &buffer;

        // SubBuffer cannot be constructed without a Buffer.
        SubBuffer() = delete;
        SubBuffer(const SubBuffer&) = delete;
        SubBuffer(SubBuffer&&) = delete;

        void store(std::span<const T> data) override {
			// TODO: check overflow only when strict mode is enabled.
            if (data.size() > size) {
                throw std::overflow_error { "Buffer overflow." };
            }

            buffer.bind();
            glBufferSubData(BufferTarget, offset, data.size() * sizeof(T), data.data());
        }
    };

    template <GLenum BufferTarget, typename T>
    struct Buffer final : BufferBase<BufferTarget, T>{
        GLuint handle;
        std::size_t capacity = 0;

        explicit Buffer(GLenum usage) : BufferBase<BufferTarget, T> { usage } {
            glGenBuffers(1, &handle);
        }
        Buffer(const Buffer&) = delete;
        Buffer(Buffer&& source) noexcept : BufferBase<BufferTarget, T> { source.usage },
                                           capacity { source.capacity }
        {
            source.handle = 0;
        }

        Buffer &operator=(const Buffer&) = delete;
        Buffer &operator=(Buffer&& source) noexcept {
            glDeleteBuffers(1, &handle);
            handle = std::exchange(source.handle, 0);
            capacity = source.capacity;
            return *this;
        }

	    ~Buffer() {
            if (handle != 0) {
                glDeleteBuffers(1, &handle);
            }
        }

		void bind() const{
			glBindBuffer(BufferTarget, handle);
        }

        void reserve(std::size_t size) {
            bind();
            glBufferData(BufferTarget, size * sizeof(T), nullptr, static_cast<GLenum>(BufferBase<BufferTarget, T>::usage));

            capacity = size;
        }

        void store(std::span<const T> data) override {
            bind();
            glBufferData(BufferTarget, data.size() * sizeof(T), data.data(), static_cast<GLenum>(BufferBase<BufferTarget, T>::usage));

            capacity = data.size();
        }

        SubBuffer<BufferTarget, T> getSubBuffer() {
            return getSubBuffer(capacity, 0);
        }

        SubBuffer<BufferTarget, T> getSubBuffer(std::size_t size, std::ptrdiff_t offset) {
            return SubBuffer<BufferTarget, T> { *this, size, offset };
        }
    };
}