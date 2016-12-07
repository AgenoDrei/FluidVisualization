#pragma once


#include <GL/glew.h>

class Camera;
class WindowHandler;
class DataSet;
class Timestep;
class Shader;

class ParticleRenderer {
public:
    ParticleRenderer();
    ~ParticleRenderer();
    void setData(Timestep* step, uint32_t count);
    void render(Camera* camera, WindowHandler* wHandler);
private:
    GLuint VAO, VBO;
    uint32_t particleCount;
    glm::vec4* buffer;
    Shader* particleShader;

};


