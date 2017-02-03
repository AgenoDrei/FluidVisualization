#pragma once


#include <vector>
#include <list>
#include "Triangle.h"

class WindowHandler;
class Camera;
class VertexArrayBuffer;
class MarchingCubesShader;
class MarchingCubesRenderObject;

class RendererMarchingCubes {
private:
    MarchingCubesShader* _shader;

    std::list<MarchingCubesRenderObject*> _objects;
public:
    RendererMarchingCubes();

    void addTriangles(const std::vector<Triangle>& triangles);
    void addVertexIndexBuffer(const std::vector<VertexPositionNormal>& vertices, const std::vector<int>& indices);

    void render(Camera *camera, WindowHandler *wHandler);
};



