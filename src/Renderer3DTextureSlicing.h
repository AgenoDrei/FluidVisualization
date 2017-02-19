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
    Renderer3DTextureSlicing(float quality, uint32_t dimX, uint32_t dimY, uint32_t dimZ, Camera* camera);
    ~Renderer3DTextureSlicing();
    void setData(Timestep* step);
    void render(WindowHandler* wHandler);
private:
    Camera* camera;
    glm::vec3 viewDirSlicing;
    uint32_t dimX, dimY, dimZ;
    GLuint VAO, VBO, texture;
    Shader* shader;
    glm::vec3 *vTextureSlices, *intersection;
    void sliceVolume();
    int num_slices, sizeof_vTextureSlices;
};
