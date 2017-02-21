#include "MarchingCubesRenderObject.h"

MarchingCubesRenderObject::MarchingCubesRenderObject(VertexArrayBuffer* vertexBuffer) {
    _numIndices = 0;
    _indexBuffer = 0;
    _vertexBuffer = vertexBuffer;
}

MarchingCubesRenderObject::MarchingCubesRenderObject(GLuint indices, GLuint IndexBuffer, VertexArrayBuffer* vertexBuffer) {
    _numIndices = indices;
    _indexBuffer = IndexBuffer;
    _vertexBuffer = vertexBuffer;
}

bool MarchingCubesRenderObject::isIndexed() const {
    return _indexBuffer != 0;
}

GLuint MarchingCubesRenderObject::getIndexBuffer() const {
    return _indexBuffer;
}

GLuint MarchingCubesRenderObject::getNumberIndices() const {
    return _numIndices;
}

VertexArrayBuffer* MarchingCubesRenderObject::getVertexBuffer() const {
    return _vertexBuffer;
}
