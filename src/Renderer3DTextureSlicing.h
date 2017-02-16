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
    void setData(Timestep* step, glm::vec3 initViewDir);
    void render(Camera* camera, WindowHandler* wHandler);
private:
    bool bViewRotated;
    uint32_t dimX, dimY, dimZ;
    GLuint VAO, VBO, texture;
    Shader* shader;
    glm::vec3* vTextureSlices;
    void sliceVolume(glm::vec3 viewDir);
    int MAX_SLICES;
//    int FindAbsMax(glm::vec3 v);
};
