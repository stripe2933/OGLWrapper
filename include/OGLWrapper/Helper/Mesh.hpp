//
// Created by gomkyung2 on 12/2/23.
//

#pragma once

#include <vector>

#include <OGLWrapper/VertexArray.hpp>
#include <OGLWrapper/Buffer.hpp>

#include <type_mapper.hpp>

#include "VertexAttributes.hpp"

namespace OGLWrapper::Helper{
    template <typename VertexT>
    struct MeshBase {
        using vertex_type = VertexT;

        std::vector<VertexT> vertices;
    };

    template <typename VertexT>
    struct GpuMesh{
        OGLWrapper::VertexArray vertex_arrray;
        OGLWrapper::Buffer<GL_ARRAY_BUFFER, VertexT> vertex_buffer;

		template <bool CheckDrawMode = OGLWRAPPER_STRICT_MODE>
        void draw(GLenum mode) const {
			if constexpr (CheckDrawMode) {
				if (!GLConstraints::isDrawMode(mode)) {
                    throw std::invalid_argument { "Invalid draw mode." };
                }
			}

            vertex_arrray.bind();
            glDrawArrays(mode, 0, vertex_buffer.capacity);
        }
    };

    template <typename VertexT>
    struct Mesh : MeshBase<VertexT>{
        explicit Mesh(std::vector<VertexT> vertices) : MeshBase<VertexT>{ std::move(vertices) } {

        }

        GpuMesh<VertexT> transferToGpu(const VertexAttributes<VertexT> &attribute_indices, GLenum usage) const {
            OGLWrapper::VertexArray vertex_array{};
            vertex_array.bind();

            OGLWrapper::Buffer<GL_ARRAY_BUFFER, VertexT> vertex_buffer{ usage };
            vertex_buffer.store(MeshBase<VertexT>::vertices);

            attribute_indices.setVertexAttribArrays();

            return { std::move(vertex_array), std::move(vertex_buffer) };
        }
    };

    template <typename VertexT, std::integral IndexT>
    class GpuElementMesh{
        static constexpr type_mapper::mapper index_type_mapper {
            type_mapper::make_mapping<std::uint8_t>(GL_UNSIGNED_BYTE),
            type_mapper::make_mapping<std::uint16_t>(GL_UNSIGNED_SHORT),
            type_mapper::make_mapping<std::uint32_t>(GL_UNSIGNED_INT),
        };

    public:
        OGLWrapper::VertexArray vertex_arrray;
        OGLWrapper::Buffer<GL_ARRAY_BUFFER, VertexT> vertex_buffer;
        OGLWrapper::Buffer<GL_ELEMENT_ARRAY_BUFFER, IndexT> index_buffer;

		template <bool CheckDrawMode = OGLWRAPPER_STRICT_MODE>
        void draw(GLenum mode) const {
			if constexpr (CheckDrawMode) {
				if (!GLConstraints::isDrawMode(mode)) {
                    throw std::invalid_argument { "Invalid draw mode." };
                }
			}

            vertex_arrray.bind();
            glDrawElements(mode, index_buffer.capacity, index_type_mapper.get<IndexT>(), nullptr);
        }
    };

    template <typename VertexT, std::integral IndexT>
    struct ElementMesh : MeshBase<VertexT>{
        using index_type = IndexT;

        std::vector<IndexT> indices;

        explicit ElementMesh(std::vector<VertexT> vertices, std::vector<IndexT> indices)
                : MeshBase<VertexT>{ std::move(vertices) },
                  indices { std::move(indices) }
        {

        }

        GpuElementMesh<VertexT, IndexT> transferToGpu(const VertexAttributes<VertexT> &attribute_indices, GLenum usage) const {
            OGLWrapper::VertexArray vertex_array{};
            vertex_array.bind();

            OGLWrapper::Buffer<GL_ARRAY_BUFFER, VertexT> vertex_buffer{ usage };
            vertex_buffer.store(MeshBase<VertexT>::vertices);

            attribute_indices.setVertexAttribArrays();

            OGLWrapper::Buffer<GL_ELEMENT_ARRAY_BUFFER, IndexT> index_buffer{ usage };
            index_buffer.store(indices);

            return { std::move(vertex_array), std::move(vertex_buffer), std::move(index_buffer) };
        }
    };
}