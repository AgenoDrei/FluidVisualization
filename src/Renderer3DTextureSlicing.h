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
    Renderer3DTextureSlicing(uint32_t dimX, uint32_t dimY, uint32_t dimZ);
    ~Renderer3DTextureSlicing();
    void setTextureData(Timestep* step);
    void setBufferData(glm::vec3* vTextureSlices);
    void updateSizeofTextureSlicesVolume(int numSlices);
    void render(Camera* camera, WindowHandler* wHandler);
    glm::vec3 viewDirOnSlicing;
private:
    Shader* shader;
    int sizeofTextureSlicesVolume;
    uint32_t dimX, dimY, dimZ;
    GLuint VAO, VBO, texture;
    void setupParamsAndBinds();
};
