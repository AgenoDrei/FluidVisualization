#pragma once


#include <vector>
#include <list>
#include "Triangle.h"
#include "Shader.h"

class WindowHandler;
class Camera;

class VertexArrayBuffer { // todo recreate with read only getters
public:
    VertexArrayBuffer();
    GLuint VAO, VBO;
    GLuint numberOfVertices;
};

class RendererMarchingCubes {
private:
    Shader* _shader;

    std::list<VertexArrayBuffer> _vertexBuffers;
public:
    RendererMarchingCubes();

    void addTriangles(const std::vector<Triangle>& triangles);

    void render(Camera *camera, WindowHandler *wHandler);
};



