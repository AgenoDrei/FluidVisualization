#pragma once


#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

class Camera;
class WindowHandler;
class DataSet;
class Timestep;
class Shader;
class SkyBox;
class Shader;

class RendererRayCasting {
public:
    RendererRayCasting(GLfloat rayStepSize, SkyBox* skyBox);
    ~RendererRayCasting();
    void setData(Timestep* step, uint32_t count);
    void render(Camera* camera, WindowHandler* wHandler);
private:
    GLuint cubeVAO, cubeVBO, cubeEBO, texture;
    uint32_t particleCount;
    glm::vec4* buffer;
    Shader* _shader;
    GLfloat _rayStepSize;
    SkyBox* _skyBox;
};


