//
// Created by hungr on 2021/02/27.
//

#ifndef GL_VERTEX_ARRAY_H
#define GL_VERTEX_ARRAY_H

#include <functional>

#include <GL/glew.h>
#include <GL/gl.h>

namespace gl {
    class vertex_array {
    public:
        class bind_context {
        public:
            explicit bind_context(std::reference_wrapper<const vertex_array> ref);
            ~bind_context();
            operator bool() const;
        private:
            std::reference_wrapper<const vertex_array> array;
        };
    public:
        vertex_array();
        explicit vertex_array(GLuint handle);
        ~vertex_array();
        [[nodiscard]] GLuint handle() const noexcept;
        [[nodiscard]] bind_context get_bind() const;
        void bind() const;
        void unbind() const;
    private:
        GLuint m_handle;
    };
}

#endif //GL_VERTEX_ARRAY_H
