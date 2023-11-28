//
// Created by gomkyung2 on 11/27/23.
//

#pragma once

#include <span>

#include <GL/gl3w.h>

namespace OGLWrapper{
    enum class BufferBindingTarget : GLenum {
        ArrayBuffer = GL_ARRAY_BUFFER, // Vertex attributes
        AtomicCounterBuffer = GL_ATOMIC_COUNTER_BUFFER, // Atomic counter storage
        CopyReadBuffer = GL_COPY_READ_BUFFER, // Buffer copy source
        CopyWriteBuffer = GL_COPY_WRITE_BUFFER, // Buffer copy destination
        DispatchIndirectBuffer = GL_DISPATCH_INDIRECT_BUFFER, // Indirect compute dispatch commands
        DrawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER, // Indirect command arguments
        ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER, // Vertex array indices
        PixelPackBuffer = GL_PIXEL_PACK_BUFFER, // Pixel read target
        PixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER, // Texture data source
        QueryBuffer = GL_QUERY_BUFFER, // Query result buffer
        ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER, // Read-write storage for shaders
        TextureBuffer = GL_TEXTURE_BUFFER, // Texture data buffer
        TransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER, // Transform feedback buffer
        UniformBuffer = GL_UNIFORM_BUFFER, // Uniform block storage
    };

    enum class BufferUsage : GLenum {
        StaticDraw = GL_STATIC_DRAW,
        StaticRead = GL_STATIC_READ,
        StaticCopy = GL_STATIC_COPY,
        DynamicDraw = GL_DYNAMIC_DRAW,
        DynamicRead = GL_DYNAMIC_READ,
        DynamicCopy = GL_DYNAMIC_COPY,
        StreamDraw = GL_STREAM_DRAW,
        StreamRead = GL_STREAM_READ,
        StreamCopy = GL_STREAM_COPY,
    };

    template <typename T>
    struct BufferBase {
        BufferBindingTarget target;
        BufferUsage usage;

        BufferBase(BufferBindingTarget target, BufferUsage usage) : target { target }, usage { usage } { }

        virtual void store(std::span<const T> data) = 0;
        virtual ~BufferBase() = default;
    };

    template <typename T>
    struct Buffer;

    template <typename T>
    class SubBuffer final : BufferBase<T> {
        std::size_t size;
        std::ptrdiff_t offset;

        template <bool CheckOverflow = STRICT_MODE>
        explicit SubBuffer(const Buffer<T> &buffer, std::size_t size, std::ptrdiff_t offset)
                : BufferBase<T> { buffer.target, buffer.usage }, buffer { buffer }, size { size }, offset { offset }
        {
            if constexpr (CheckOverflow) {
                if (offset + size > buffer.capacity) {
                    throw std::out_of_range { "SubBuffer should be within the range of the buffer." };
                }
            }
        }

        friend struct Buffer<T>;

    public:
        const Buffer<T> &buffer;

        // SubBuffer cannot be constructed without a Buffer.
        SubBuffer() = delete;
        SubBuffer(const SubBuffer&) = delete;
        SubBuffer(SubBuffer&&) = delete;

        void store(std::span<const T> data) override {
            if (data.size() > size) {
                throw std::overflow_error { "Buffer overflow." };
            }

            glBindBuffer(static_cast<GLenum>(buffer.target), buffer.handle);
            glBufferSubData(static_cast<GLenum>(buffer.target), offset, data.size() * sizeof(T), data.data());
        }
    };

    template <typename T>
    struct Buffer final : BufferBase<T>{
        GLuint handle;
        std::size_t capacity = 0;

        explicit Buffer(BufferBindingTarget target, BufferUsage usage) : BufferBase<T> { target, usage } {
            glGenBuffers(1, &handle);
        }
        Buffer(const Buffer&) = delete;
        Buffer(Buffer&& source) noexcept
                : BufferBase<T> { source.target, source.usage },
                  capacity { source.capacity }
        {
            source.handle = 0;
        }

        void reserve(std::size_t size) {
            glBindBuffer(static_cast<GLenum>(BufferBase<T>::target), handle);
            glBufferData(static_cast<GLenum>(BufferBase<T>::target), size * sizeof(T), nullptr, static_cast<GLenum>(BufferBase<T>::usage));

            capacity = size;
        }

        void store(std::span<const T> data) override {
            glBindBuffer(static_cast<GLenum>(BufferBase<T>::target), handle);
            glBufferData(static_cast<GLenum>(BufferBase<T>::target), data.size() * sizeof(T), data.data(), static_cast<GLenum>(BufferBase<T>::usage));

            capacity = data.size();
        }

        SubBuffer<T> getSubBuffer() {
            return getSubBuffer(capacity, 0);
        }

        SubBuffer<T> getSubBuffer(std::size_t size, std::ptrdiff_t offset) {
            return SubBuffer<T> { *this, size, offset };
        }
    };
}