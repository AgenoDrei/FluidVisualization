#include <iostream>
#include "TextureSlicingRenderer.h"

#include "DataManagement/DataSet.h"
#include "DataManagement/Timestep.h"
#include "WindowHandler.h"
#include "Shader/Shader.h"

TextureSlicingRenderer::TextureSlicingRenderer(uint32_t dimX, uint32_t dimY, uint32_t dimZ) {
    shader = new Shader("shader/textureSlicer.vert", "shader/textureSlicer.frag");
    TextureSlicingRenderer::sizeofTextureSlicesVolume = 42*12*sizeof(glm::vec3);  // default numSlices is 42
    TextureSlicingRenderer::dimX = dimX;
    TextureSlicingRenderer::dimY = dimY;
    TextureSlicingRenderer::dimZ = dimZ;

    setupParamsAndBinds();
}

TextureSlicingRenderer::~TextureSlicingRenderer() {
    delete shader;
}

void TextureSlicingRenderer::updateSizeofTextureSlicesVolume(int numSlices) {
    int newSizeofTSV = numSlices*12*sizeof(glm::vec3);

    TextureSlicingRenderer::sizeofTextureSlicesVolume = newSizeofTSV;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeofTextureSlicesVolume, 0, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);       //enable vertex attribute array for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);
}

void TextureSlicingRenderer::setTextureData(Timestep *step) {
    auto particleCount = step->getSize();
    auto pData = new float[particleCount];
    for (auto i = 0u; i < particleCount; i++) {
        pData[i] = step->getParticle(i).density * 1000;   // *1000 s.t. values big enough to get integer red value
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, dimX, dimY, dimZ, 0, GL_RED, GL_FLOAT, pData);
    glGenerateMipmap(GL_TEXTURE_3D);

    delete [] pData;
}

void TextureSlicingRenderer::setBufferData(glm::vec3 *vTextureSlices) {
    //update buffer object with the new vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeofTextureSlicesVolume, &(vTextureSlices[0].x));
}

void TextureSlicingRenderer::render(BaseCamera* camera, WindowHandler* wHandler) {
    glm::mat4 model, view, projection;
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    view = camera->GetViewMatrix();
    projection = camera->GetProjectonMatrix(wHandler, 0.1f, 10.0f);

    glBindVertexArray(VAO);
    shader->use();
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//    glUniform1i(glGetUniformLocation(shader->Program, "volume"), 0);
    glDrawArrays(GL_TRIANGLES, 0, sizeofTextureSlicesVolume / sizeof(glm::vec3));
    shader->unUse();
}
