#pragma once


#include <vector>
#include <list>
#include "Triangle.h"

class WindowHandler;
class Camera;
class VertexArrayBuffer;
class MarchingCubesShader;

class RendererMarchingCubes {
private:
    MarchingCubesShader* _shader;

    std::list<VertexArrayBuffer*> _vertexBuffers;
public:
    RendererMarchingCubes();

    void addTriangles(const std::vector<Triangle>& triangles);

    void render(Camera *camera, WindowHandler *wHandler);
};



