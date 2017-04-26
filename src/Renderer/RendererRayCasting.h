#pragma once


#include <GL/glew.h>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <Texture.h>

class Camera;
class WindowHandler;
class DataSet;
class Timestep;
class Shader;
class SkyBox;
class Shader;
class Texture;

class RendererRayCasting {
public:
    RendererRayCasting(GLfloat rayStepSize, SkyBox* skyBox);
    ~RendererRayCasting();
    void setData(Timestep* step, uint32_t count);
    void render(Camera* camera, WindowHandler* wHandler);
    void changeStepSize(GLfloat value);
    void changeLightPos();
    void createShaderRandomValues();
    void toggleReflection();
    void toggleShadow();
private:
    GLuint _cubeVAO, _cubeVBO, _cubeEBO;
    Texture* _texture;
    Texture* _randomTexture;
    uint32_t _particleCount;
    Shader* _shader;
    GLfloat _rayStepSize;
    SkyBox* _skyBox;
    glm::vec3 _lightPos;
    bool _reflection, _shadow;
};


