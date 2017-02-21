#pragma once

#include <vector>
#include <list>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include "Triangle.h"

class WindowHandler;
class BaseCamera;
class VertexArrayBuffer;
class MarchingCubesShader;
class MarchingCubesRenderObject;
class SkyBox;

class TextureRenderer;

class RendererMarchingCubes {
private:
    MarchingCubesShader* _shader;
    SkyBox* _skyBox;

    GLuint _reflectionTexture, _reflectionFramebuffer, _reflectionDepthBuffer;

    std::list<MarchingCubesRenderObject*> _objects;

    void renderReflectionMap(BaseCamera *camera, WindowHandler *wHandler);

    TextureRenderer* _debugRenderer;
public:
    RendererMarchingCubes(SkyBox* skyBox);

    void addTriangles(const std::vector<Triangle>& triangles);
    void addVertexIndexBuffer(const std::vector<VertexPositionNormal>& vertices, const std::vector<int>& indices);

    void render(BaseCamera *camera, WindowHandler *wHandler);

    void clean();
};



