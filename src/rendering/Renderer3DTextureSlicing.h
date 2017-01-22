#pragma once
#include <GL/glew.h>
#include <glm/ext.hpp>

class Camera;
class WindowHandler;
class DataSet;
class Timestep;
class Shader;

class Renderer3DTextureSlicing {
public:
    Renderer3DTextureSlicing();
    ~Renderer3DTextureSlicing();
    void setData(Timestep* step, uint32_t count);
    void render(Camera* camera, WindowHandler* wHandler);
private:
    uint32_t particleCount, dimX, dimY, dimZ;
    GLuint VAO, VBO, textureID;
    Shader* shader;
    glm::vec4* buffer;
    glm::vec3* vTextureSlices;
    void sliceVolume();
};
