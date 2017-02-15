#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

class GroundShader;
class VertexArrayBuffer;
class WindowHandler;
class Camera;

class Ground {
protected:
    GroundShader* _shader;
    VertexArrayBuffer* _vertexBuffer;
    GLuint _indexBuffer;
    GLuint _texture;
public:
    Ground();

    void render(Camera *camera, WindowHandler *wHandler);
};
