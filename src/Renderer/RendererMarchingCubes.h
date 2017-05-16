#pragma once

#include <vector>
#include <list>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/detail/type_mat.hpp>
#include <Shader/Shader.h>
#include <Shader/MVPShader.h>
#include "Algorithms/MarchingCubes/Triangle.h"

class WindowHandler;
class BaseCamera;
class VertexArrayBuffer;
class MarchingCubesShader;
class MarchingCubesRenderObject;
class SkyBox;
class ShadowMapShader;
class TextureRenderer;
class RendererDebugQuad;
class MVPShader;
class DirectionalLight;

class RendererMarchingCubes {
private:
    MarchingCubesShader* _shader;
    ShadowMapShader* _shadowShader;
    SkyBox* _skyBox;
    RendererDebugQuad* _debug;

    GLuint _reflectionTexture, _reflectionFramebuffer, _reflectionDepthBuffer, _shadowMapFramebuffer, _depthTexture;

    std::list<MarchingCubesRenderObject*> _objects;

    void renderReflectionMap(BaseCamera *camera, WindowHandler *wHandler);
    void renderShadowMap(WindowHandler *wHandler);

    void renderWithShadow(BaseCamera *camera, WindowHandler *wHandler);

    TextureRenderer* _debugRenderer;

    DirectionalLight* _light;
public:
    RendererMarchingCubes(SkyBox* skyBox);

    void toggleReflection();
    void toggleShadow();

    void addTriangles(const std::vector<Triangle>& triangles);
    void addVertexIndexBuffer(const std::vector<VertexPositionNormal>& vertices, const std::vector<int>& indices);

    void render(BaseCamera *camera, WindowHandler *wHandler);

    void clean();

    glm::mat4* getShadowMVP();

    void setLight(DirectionalLight* light);
};



