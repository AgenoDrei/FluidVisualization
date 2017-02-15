#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>

class SkyBoxShader;
class VertexArrayBuffer;
class WindowHandler;
class Camera;

class SkyBox {
private:
    GLuint _texture;
protected:
    SkyBoxShader* _shader;

    VertexArrayBuffer* _vertexBuffer;

    void loadSkyBox(std::vector<const GLchar*> faces);
public:
    SkyBox();

    void render(Camera *camera, WindowHandler *wHandler);
};



