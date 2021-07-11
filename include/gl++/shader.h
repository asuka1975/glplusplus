//
// Created by hungr on 2021/02/27.
//

#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <string>

#include <GL/glew.h>

namespace gl {
    class shader_program {
    public:
        shader_program();
        shader_program(const shader_program& obj) = delete;
        shader_program(shader_program&& obj) noexcept ;
        ~shader_program();
        [[nodiscard]] bool enabled() const noexcept;
        [[nodiscard]] GLuint handle() const noexcept;
        void reset();
        bool add_shader(const std::string &source, GLenum type);
        bool link();
        void use() const;
        void unuse() const;
    private:
        GLuint m_handle;
    };
}

#endif //GL_SHADER_H
