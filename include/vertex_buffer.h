//
// Created by hungr on 2021/02/27.
//

#ifndef GL_VERTEX_BUFFER_H
#define GL_VERTEX_BUFFER_H

#include <iterator>
#include <type_traits>
#include <vector>

#include <GL/glew.h>

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

    class class_t{};
    template <class T>
    using value_t = std::conditional_t<std::is_class_v<T>, T, class_t>;

    template <class Traits>
    class vertex_buffer {
    public:
        using value_type = typename Traits::value_type;
        static constexpr GLenum buffer_target = Traits::target;
        static constexpr GLenum buffer_usage = Traits::usage;
    private:
        template <class Iterator, class IteratorTraits=std::iterator_traits<Iterator>>
        static constexpr bool is_input_iterator_v = std::conjunction_v<
                std::is_base_of<std::input_iterator_tag, typename IteratorTraits::iterator_category>,
                std::is_convertible<typename IteratorTraits::value_type, value_type>,
                std::negation<std::is_base_of<std::random_access_iterator_tag, typename IteratorTraits::iterator_category>>>;
        template <class Iterator, class IteratorTraits=std::iterator_traits<Iterator>>
        static constexpr bool is_random_access_iterator_v = std::conjunction_v<
                std::is_base_of<std::random_access_iterator_tag, typename IteratorTraits::iterator_category>,
                std::is_convertible<typename IteratorTraits::value_type, value_type>>;
    public:
        template <class Iterator, class=std::enable_if_t<is_input_iterator_v<Iterator>>>
        vertex_buffer(const Iterator& begin, const Iterator& end) : m_size(std::distance(begin, end)), m_handle(0) {
            std::vector<value_type> data(begin, end);
            glGenBuffers(1, &m_handle);
            glBindBuffer(buffer_target, m_handle);
            glBufferData(buffer_target, sizeof(value_type) * data.size(), data.data(), buffer_usage);
        }
        vertex_buffer(const vertex_buffer<Traits>&) = delete;
        vertex_buffer(vertex_buffer<Traits>&& obj)  noexcept : m_size(obj.m_size), m_handle(obj.m_handle) {
            obj.m_handle = 0;
        }
        vertex_buffer<Traits>& operator=(const vertex_buffer<Traits>&) = delete;
        vertex_buffer<Traits>& operator=(vertex_buffer<Traits>&& obj) noexcept {
            if(this != &obj) {
                m_size = obj.m_size;
                glDeleteBuffers(1, &m_handle);
                m_handle = obj.m_handle;
                obj.m_handle = 0;
            }
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
        void vertex_pointer(GLuint location, GLboolean normalized, T value_t<value_type>::*member) {
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
        template <class Iterator>
        auto modify(std::ptrdiff_t offset, const Iterator& begin, const Iterator& end) -> std::enable_if_t<is_input_iterator_v<Iterator>> {
            bind();
            std::vector<value_type> data(begin, end);
            glBufferSubData(buffer_target, offset * sizeof(value_type), data.size() * sizeof(value_type), data.data());
        }
        template <class Iterator>
        auto modify(std::ptrdiff_t offset, const Iterator& begin, const Iterator& end) -> std::enable_if_t<is_random_access_iterator_v<Iterator>> {
            bind();
            glBufferSubData(buffer_target, offset * sizeof(value_type), std::distance(begin, end) * sizeof(value_type), begin);
        }
        template <class Iterator>
        void modify(const Iterator& begin, const Iterator& end) {
            modify(0, begin, end);
        }
        template <class Iterator>
        auto extend(std::ptrdiff_t offset, const Iterator& begin, const Iterator& end) {
            auto size = std::distance(begin, end) + offset;
            auto expect_size = size;
            if(size > m_size) {
                for(std::size_t i = 1; i != expect_size; ) {
                    if(i < expect_size) i <<= 1;
                    if(i > expect_size) expect_size = i;
                }
                GLuint vbo;
                glGenBuffers(1, &vbo);
                glBindBuffer(buffer_target, vbo);
                glBufferData(buffer_target, expect_size * sizeof(value_type), nullptr, buffer_usage);
                glBindBuffer(GL_COPY_READ_BUFFER, m_handle);
                glBindBuffer(GL_COPY_WRITE_BUFFER, vbo);
                glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, m_size * sizeof(value_type));
                glDeleteBuffers(1, m_handle);
                m_handle = vbo;
            }
            modify(offset, begin, end);
        }
        [[nodiscard]] std::size_t size() const noexcept {
            return m_size;
        }
        template <class Iterator>
        auto get(std::ptrdiff_t offset, const Iterator& begin, const Iterator& end) -> std::enable_if_t<is_input_iterator_v<Iterator>> {
            bind();
            std::vector<value_type> data(begin, end);
            glGetBufferSubData(buffer_target, offset, data.size() * sizeof(value_type), data.data());
        }
        template <class Iterator>
        auto get(std::ptrdiff_t offset, const Iterator& begin, const Iterator& end) -> std::enable_if_t<is_random_access_iterator_v<Iterator>> {
            bind();
            glGetBufferSubData(buffer_target, offset, std::distance(begin, end), begin);
        }
        template <class Iterator>
        void get(const Iterator& begin, const Iterator& end) {
            get(0, begin, end);
        }
    private:
        std::size_t m_size;
        GLuint m_handle;
    };
}

#endif //GL_VERTEX_BUFFER_H
