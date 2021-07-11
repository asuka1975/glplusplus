//
// Created by hungr on 2021/02/27.
//

#ifndef GL_PRIMITIVE_TYPE_H
#define GL_PRIMITIVE_TYPE_H

#include <GL/gl.h>
#include <glm/glm.hpp>

namespace gl {
    template <class T>
    struct non_value {};
    template <class T>
    struct gl_primitive_type {
        using type = void;
        inline static constexpr GLenum value = non_value<T>::value;
    };
    template <>
    struct gl_primitive_type<GLuint> {
        using type = GLuint;
        inline static constexpr GLenum value = GL_UNSIGNED_INT;
    };
    template <>
    struct gl_primitive_type<GLint> {
        using type =GLint;
        inline static constexpr GLenum value = GL_INT;
    };
    template <>
    struct gl_primitive_type<GLushort> {
        using type = GLushort;
        inline static constexpr GLenum value = GL_UNSIGNED_SHORT;
    };
    template <>
    struct gl_primitive_type<GLshort> {
        using type = GLshort;
        inline static constexpr GLenum value = GL_SHORT;
    };
    template <>
    struct gl_primitive_type<GLubyte> {
        using type = GLubyte;
        inline static constexpr GLenum value = GL_UNSIGNED_BYTE;
    };
    template <>
    struct gl_primitive_type<GLbyte> {
        using type = GLbyte;
        inline static constexpr GLenum value = GL_BYTE;
    };
    template <>
    struct gl_primitive_type<GLfloat> {
        using type = GLfloat;
        inline static constexpr GLenum value = GL_FLOAT;
    };
    template <>
    struct gl_primitive_type<GLdouble> {
        using type = GLdouble;
        inline static constexpr GLenum value = GL_DOUBLE;
    };
    template <glm::length_t C, class T, glm::qualifier Q>
    struct gl_primitive_type<glm::vec<C, T, Q>> {
        using type = T;
        inline static constexpr GLenum value = gl_primitive_type<T>::value;
    };
}

#endif //GL_PRIMITIVE_TYPE_H
