#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

class WindowHandler;
class BaseCamera;
class Shader;

class TextureRenderer {
private:
    Shader* _shader;
    GLuint _texture, _vao, _vbo;
public:
    TextureRenderer(GLuint texture);

    void render(BaseCamera *camera, WindowHandler *wHandler);
};



