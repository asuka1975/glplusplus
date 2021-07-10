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
        inline static constexpr  GLenum value = GL_UNSIGNED_INT;
    };
    template <>
    struct gl_primitive_type<GLushort> {
        using type = GLuint;
        inline static constexpr GLenum value = GL_UNSIGNED_SHORT;
    };
    template <>
    struct gl_primitive_type<GLubyte> {
        using type = GLuint;
        inline static constexpr GLenum value = GL_UNSIGNED_BYTE;
    };
    template <>
    struct gl_primitive_type<GLfloat> {
        using type = GLfloat;
        inline static constexpr GLenum value = GL_FLOAT;
    };
    template <>
    struct gl_primitive_type<glm::vec2> {
        using type = GLfloat;
        inline static constexpr GLenum value = GL_FLOAT;
    };
    template <>
    struct gl_primitive_type<glm::vec3> {
        using type = GLfloat;
        inline static constexpr GLenum value = GL_FLOAT;
    };
    template <>
    struct gl_primitive_type<glm::vec4> {
        using type = GLfloat;
        inline static constexpr GLenum value = GL_FLOAT;
    };
}

#endif //GL_PRIMITIVE_TYPE_H
