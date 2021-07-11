//
// Created by hungr on 2021/02/27.
//
#include "gl++/shader.h"

#include <iostream>

gl::shader_program::shader_program() {
    m_handle = glCreateProgram();
}

gl::shader_program::~shader_program() {
    reset();
}

bool gl::shader_program::enabled() const noexcept {
    return m_handle != 0;
}

GLuint gl::shader_program::handle() const noexcept {
    return m_handle;
}

gl::shader_program::shader_program(shader_program &&obj) noexcept {
    if(this != &obj) {
        reset();
        m_handle = obj.m_handle;
        obj.m_handle = 0;
    }
}

void gl::shader_program::reset() {
    if(enabled()) {
        glDeleteProgram(handle());
        m_handle = 0;
    }
}

bool gl::shader_program::add_shader(const std::string &src, GLenum type) {
    if(!enabled()) return false;
    GLuint shader = glCreateShader(type);
    auto source = src.data();
    GLint length = src.size();
    glShaderSource(shader, 1, &source, &length);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        GLsizei size; glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
        std::string log(size, 0);
        glGetShaderInfoLog(shader, log.length(), &size, log.data());
        std::cerr << log << std::endl;
        return false;
    }

    glAttachShader(handle(), shader);
    glDeleteShader(shader);

    return true;
}

bool gl::shader_program::link() {
    if(!enabled()) return false;
    glLinkProgram(handle());
    GLint status;
    glGetProgramiv(handle(), GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        GLsizei size; glGetProgramiv(handle(), GL_INFO_LOG_LENGTH, &size);

        std::string log(size, 0);
        glGetProgramInfoLog(handle(), log.length(), &size, log.data());
        std::cerr << log << std::endl;
        return false;
    }

    return true;
}

void gl::shader_program::use() const {
    glUseProgram(handle());
}

void gl::shader_program::unuse() const {
    glUseProgram(0);
}

