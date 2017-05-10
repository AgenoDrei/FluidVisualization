#include <iostream>
#include <SkyBox.h>
#include "RendererTextureSlicing.h"

#include "DataManagement/DataSet.h"
#include "DataManagement/Timestep.h"
#include "WindowHandler.h"
#include "Shader/EffectShader.h"

TextureSlicingRenderer::TextureSlicingRenderer(uint32_t dimX, uint32_t dimY, uint32_t dimZ, SkyBox* skyBox) {
    _shader = new EffectShader("shader/textureSlicer.vert", "shader/textureSlicer.frag");
    _skyBox = skyBox;
    TextureSlicingRenderer::_sizeofTextureSlicesVolume = 42*12*sizeof(glm::vec3);  // default numSlices is 42
    TextureSlicingRenderer::_dimX = dimX;
    TextureSlicingRenderer::_dimY = dimY;
    TextureSlicingRenderer::_dimZ = dimZ;

    setupParamsAndBinds();
}

TextureSlicingRenderer::~TextureSlicingRenderer() {
    delete _shader;
}

void TextureSlicingRenderer::updateSizeofTextureSlicesVolume(int numSlices) {
    int newSizeofTSV = numSlices*12*sizeof(glm::vec3);

    TextureSlicingRenderer::_sizeofTextureSlicesVolume = newSizeofTSV;

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, newSizeofTSV, 0, GL_DYNAMIC_DRAW);
}

void TextureSlicingRenderer::setupParamsAndBinds() {
    // setup texture parameters
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);      // GL_CLAMP_TO_BORDER instead of GL_CLAMP resolved GL_INVALID_ENUM error
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 4);

    // setup the vertex array and buffer objects
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);
    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _sizeofTextureSlicesVolume, 0, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);       //enable vertex attribute array for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);
}

void TextureSlicingRenderer::setTextureData(Timestep *step) {
    auto particleCount = step->getSize();
    float maxDensity = step->getMaxDensity();
    auto pData = new float[particleCount];
    for (auto i = 0u; i < particleCount; i++) {
        pData[i] = step->getParticle(i).density * (1.0f/maxDensity);
    }

    glGenTextures(1, &_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, _texture);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, _dimX, _dimY, _dimZ, 0, GL_RED, GL_FLOAT, pData);
    glGenerateMipmap(GL_TEXTURE_3D);

    delete [] pData;
}

void TextureSlicingRenderer::setBufferData(glm::vec3 *vTextureSlices) {
    //update buffer object with the new vertices
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _sizeofTextureSlicesVolume, &(vTextureSlices[0].x));
}

void TextureSlicingRenderer::toggleReflection() {
    _shader->use();
    _shader->toggleReflection();
    _shader->unUse();
}

void TextureSlicingRenderer::render(BaseCamera* camera, WindowHandler* wHandler) {
    glm::mat4 model = glm::mat4();

    _shader->use();

    _shader->setModelViewProjection(model, camera, wHandler);
    glUniform1f(glGetUniformLocation(_shader->Program, "alphaFactorInc"), 50.0f);
    glUniform1i(glGetUniformLocation(_shader->Program, "volume"), 0);
    glUniform1i(glGetUniformLocation(_shader->Program, "cube_texture"), 1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _skyBox->getTexturePointer());

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, _sizeofTextureSlicesVolume / sizeof(glm::vec3));
    glBindVertexArray(0);

    _shader->unUse();
}
