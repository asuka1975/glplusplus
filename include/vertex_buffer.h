//
// Created by hungr on 2021/02/27.
//

#ifndef GL_VERTEX_BUFFER_H
#define GL_VERTEX_BUFFER_H

#include <iterator>
#include <type_traits>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

#include "primitive_type.h"

namespace gl {
    template <GLenum V>
    inline constexpr bool is_buffer_target =
            V == GL_ARRAY_BUFFER ||
            V == GL_COPY_READ_BUFFER ||
            V == GL_COPY_WRITE_BUFFER ||
            V == GL_ELEMENT_ARRAY_BUFFER ||
            V == GL_PIXEL_PACK_BUFFER ||
            V == GL_PIXEL_UNPACK_BUFFER ||
            V == GL_TRANSFORM_FEEDBACK_BUFFER ||
            V == GL_UNIFORM_BUFFER ||
            V == GL_DRAW_INDIRECT_BUFFER ||
            V == GL_SHADER_STORAGE_BUFFER;

    template <GLenum V>
    inline constexpr bool is_buffer_usage =
            V == GL_STREAM_DRAW ||
            V == GL_STREAM_READ ||
            V == GL_STREAM_COPY ||
            V == GL_STATIC_DRAW ||
            V == GL_STATIC_READ ||
            V == GL_STATIC_COPY ||
            V == GL_DYNAMIC_DRAW ||
            V == GL_DYNAMIC_READ ||
            V == GL_DYNAMIC_COPY;

    template <class T, GLenum BufferTarget, GLenum BufferUsage,
            class=std::enable_if_t<std::is_trivially_copyable_v<T>>,
            class=std::enable_if_t<is_buffer_target<BufferTarget>>,
            class=std::enable_if_t<is_buffer_usage<BufferUsage>>>
    class buffer_trait {
    public:
        using value_type = T;
        static constexpr GLenum target = BufferTarget;
        static constexpr GLenum usage = BufferUsage;
    };

    class this_select_t {};
    static inline constexpr this_select_t this_select;

    template <class Traits>
    class vertex_buffer {
    public:
        using value_type = typename Traits::value_type;
        static constexpr GLenum buffer_target = Traits::target;
        static constexpr GLenum buffer_usage = Traits::usage;
    private:
        template <class Iterator>
        static constexpr bool is_input_iterator_v = std::conjunction_v<
                std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>,
                std::is_convertible<typename std::iterator_traits<Iterator>::value_type, value_type>>;
    public:
        template <class Iterator, class=std::enable_if_t<is_input_iterator_v<Iterator>>>
        vertex_buffer(const Iterator& begin, const Iterator& end) : data(begin, end), m_handle(0) {
            glGenBuffers(1, &m_handle);
            glBindBuffer(buffer_target, m_handle);
            glBufferData(buffer_target, sizeof(value_type) * data.size(), data.data(), buffer_usage);
        }
        ~vertex_buffer() {
            if(m_handle) {
                glDeleteBuffers(1, &m_handle);
                m_handle = 0;
            }
        }
        void vertex_pointer(GLuint location, GLboolean normalized, this_select_t this_) {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, sizeof(value_type) / sizeof(typename gl_primitive_type<value_type>::type),
                                  gl_primitive_type<value_type>::value, normalized, 0, 0);
        }
        template <class T>
        void vertex_pointer(GLuint location, GLboolean normalized, T value_type::*member) {
            glEnableVertexAttribArray(location);
            auto start = reinterpret_cast<std::size_t>(&reinterpret_cast<char const volatile&>(((value_type*)nullptr)->*member));
            glVertexAttribPointer(location, sizeof(T) / sizeof(typename gl_primitive_type<T>::type),
                                  gl_primitive_type<T>::value, normalized, sizeof(value_type),
                                  reinterpret_cast<void*>(start));
        }
        [[nodiscard]] GLuint handle() const noexcept {
            return m_handle;
        }
        void bind() {
            glBindBuffer(buffer_target, m_handle);
        }
        void unbind() {
            glBindBuffer(buffer_target, 0);
        }
        template <class Iterator, class=std::enable_if_t<is_input_iterator_v<Iterator>>>
        void modify(const Iterator& begin, const Iterator& end) {
            bind();
            auto* d = reinterpret_cast<value_type*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, data.size() * sizeof(value_type), GL_WRITE_ONLY));
            std::size_t j = 0;
            for(auto i = begin; i != end; std::advance(i, 1)) {
                d[j] = *i;
                j++;
                if(j < data.size()) break;
            }
        }
    private:
        std::vector<value_type> data;
        GLuint m_handle;
    };
}

#endif //GL_VERTEX_BUFFER_H
