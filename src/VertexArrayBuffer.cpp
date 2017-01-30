#include "VertexArrayBuffer.h"

VertexArrayBuffer::VertexArrayBuffer(GLuint numberOfVertices) :
    _numberOfVertices(numberOfVertices) {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
}

GLuint VertexArrayBuffer::getSize() {
    return _numberOfVertices;
}

GLuint VertexArrayBuffer::getVAO() {
    return _vao;
}

GLuint VertexArrayBuffer::getVBO() {
    return _vbo;
}
