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
    void render(Timestep* step, uint32_t count, Camera* camera, WindowHandler* wHandler);
private:
    uint32_t particleCount, dimX, dimY, dimZ;
    GLuint VAO, VBO;
//    glm::vec4* buffer;
    Shader* shader;
    GLuint textureID;
    glm::vec3* vTextureSlices;
    void loadIntoTexture(Timestep* step);
    void bindVertex();
    void sliceVolume();
};
