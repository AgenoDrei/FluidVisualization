#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

class VertexArrayBuffer;

class MarchingCubesRenderObject {
private:
    GLuint _numIndices;
    VertexArrayBuffer* _vertexBuffer;
    GLuint _indexBuffer;
public:
    MarchingCubesRenderObject(VertexArrayBuffer* vertexBuffer);
    MarchingCubesRenderObject(GLuint indices, GLuint IndexBuffer, VertexArrayBuffer* vertexBuffer);

    bool isIndexed() const;
    GLuint getIndexBuffer() const;
    GLuint getNumberIndices() const;
    VertexArrayBuffer* getVertexBuffer() const;
};



