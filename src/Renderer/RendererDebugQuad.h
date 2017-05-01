#pragma once

#include <GL/glew.h>
#include <glm/vec4.hpp>

class BaseCamera;
class WindowHandler;
class DataSet;
class Timestep;
class Shader;

class RendererDebugQuad {
public:
    RendererDebugQuad();
    ~RendererDebugQuad();
    void setData(Timestep* step, uint32_t count);
    void render(BaseCamera* camera, WindowHandler* wHandler);
    void render(BaseCamera *camera, WindowHandler *wHandler, GLuint textureId);
private:
    GLuint VAO, VBO, texture;
    glm::vec4* buffer;
    Shader* shader;
    uint32_t verticeCount;
};