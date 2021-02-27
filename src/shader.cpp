//
// Created by hungr on 2021/02/27.
//
#include "shader.h"

#include <fstream>
#include <iostream>
#include <vector>

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

bool gl::shader_program::add_shader(const std::string &file, GLenum type) {
    if(!enabled()) return false;
    std::ifstream fin(file);
    if(fin.fail()) {
        std::cerr << "failed to open " << file << std::endl;
    }
    std::vector<GLchar> content;
    std::copy(std::istreambuf_iterator<GLchar>(fin), std::istreambuf_iterator<GLchar>(), std::back_inserter(content));
    GLuint shader = glCreateShader(type);
    auto source = content.data();
    GLint length = content.size();
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

