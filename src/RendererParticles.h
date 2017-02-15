#pragma once


#include <GL/glew.h>

class Camera;
class WindowHandler;
class DataSet;
class Timestep;
class Shader;

class RendererParticles {
public:
    RendererParticles();
    ~RendererParticles();
    void setData(Timestep* step, uint32_t count);
    void render(Camera* camera, WindowHandler* wHandler);
private:
    GLuint VAO, VBO;
    uint32_t particleCount;
    glm::vec4* buffer;
    Shader* shader;

};


