# gl++

The gl++ is the wrapper library for OpenGL providing easier OpenGL handles and interfaces. 
The main function is smart memory management by using RAII.
It frees you from cumbersome memory management. 

# Features

## ・Vertex Buffer Object

Look at the code below. This is the traditional style of OpenGL programming. 
It is very complex flow because of stride and offset calculation, redundant buffer registration, and explicit its deletion.

```c++
// initialize vbo
GLuint vbo;
glGenBuffers(1, &vbo);
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);


// use vbo
glEnableVertexAttribArray(location);
auto stride = ...;
auto offset = ...;
glVertexAttribPointer(location, size, type, normalized, stride, offset);
glDrawArrays(GL_XXX, start, count);

// finalize vbo
glDeleteBuffers(1, &vbo);
```

Fixing it with gl++, It becomes the code below. 
The usage of vbo is easier by implicit stride and offset calculation and its deletion.
**Note that the array of vertex data is defined as interleave storage.**
```c++
// initialize vbo
using traits = gl::buffer_trait<VertexType, GL_ARRAY_BUFFER, GL_STATIC_DRAW>;
gl::vertex_buffer<traits> vbo(data.begin(), data.end());

// use vbo
vbo.bind();
vbo.vertex_pointer(0, GL_FALSE, &VertexType::member);
glDrawArrays(GL_XXX, start, count);

// no finalization
```

## ・Vertex Array Object

Look at the following code. You can acquire the temporary permission of editing vao.
Thus, you can control the range of vao editing in a straightforward appearance.

```c++
gl::vertex_array vao;
if(auto ctx = vao.get_bind()) {
    vbo1.bind();
    vbo1.vertex_pointer(0, ...);
    vbo1.vertex_pointer(1, ...);
    vbo2.vertex_pointer(2, ...);
}

// use vao
vao.use();
```

