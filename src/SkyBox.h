#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <glm/mat4x4.hpp>

class SkyBoxShader;
class VertexArrayBuffer;
class WindowHandler;
class BaseCamera;

class SkyBox {
private:
    GLuint _texture;
protected:
    SkyBoxShader* _shader;

    VertexArrayBuffer* _vertexBuffer;

    void loadSkyBox(std::vector<const GLchar*> faces);
public:
    SkyBox();

    void render(BaseCamera *camera, WindowHandler *wHandler);
    void render(BaseCamera *camera, WindowHandler *wHandler, glm::mat4x4 model);

    GLuint getTexturePointer();
};



