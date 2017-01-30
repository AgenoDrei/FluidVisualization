#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

class VertexArrayBuffer {
private:
    GLuint _vao, _vbo;
    GLuint _numberOfVertices;
public:
    VertexArrayBuffer(GLuint numberOfVertices);

    GLuint getSize();
    GLuint getVAO();
    GLuint getVBO();
};
