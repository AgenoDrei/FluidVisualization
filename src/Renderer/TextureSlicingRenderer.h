#pragma once
#include <GL/glew.h>
#include <glm/ext.hpp>

class BaseCamera;
class WindowHandler;
class DataSet;
class Timestep;
class ReflectionShader;
class SkyBox;

class TextureSlicingRenderer {
public:
    TextureSlicingRenderer(uint32_t dimX, uint32_t dimY, uint32_t dimZ, SkyBox* skyBox);
    ~TextureSlicingRenderer();
    void setTextureData(Timestep* step);
    void setBufferData(glm::vec3* vTextureSlices);
    void updateSizeofTextureSlicesVolume(int numSlices);
    void render(BaseCamera* camera, WindowHandler* wHandler);
    void toggleReflection();
    glm::vec3 viewDirOnSlicing;
private:
    SkyBox* _skyBox;
    ReflectionShader* _shader;
    int _sizeofTextureSlicesVolume;
    uint32_t _dimX, _dimY, _dimZ;
    GLuint _VAO, _VBO, _texture;
    void setupParamsAndBinds();
};
