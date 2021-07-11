//
// Created by hungr on 2021/02/27.
//
#include "gl++/vertex_array.h"


gl::vertex_array::bind_context::bind_context(std::reference_wrapper<const vertex_array> ref) : array(ref) {
    array.get().bind();
}

gl::vertex_array::bind_context::~bind_context() {
    array.get().unbind();
}

gl::vertex_array::bind_context::operator bool() const {
    return true;
}

gl::vertex_array::vertex_array() {
    glGenVertexArrays(1, &m_handle);
}

gl::vertex_array::vertex_array(GLuint handle) : m_handle(handle) {

}

gl::vertex_array::~vertex_array() {
    if(m_handle) {
        glDeleteVertexArrays(1, &m_handle);
        m_handle = false;
    }
}

GLuint gl::vertex_array::handle() const noexcept {
    return m_handle;
}

gl::vertex_array::bind_context gl::vertex_array::get_bind() const {
    return gl::vertex_array::bind_context(*this);
}

void gl::vertex_array::bind() const {
    glBindVertexArray(m_handle);
}

void gl::vertex_array::unbind() const {
    glBindVertexArray(0);
}
