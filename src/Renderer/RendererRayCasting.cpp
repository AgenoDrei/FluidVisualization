//
// Created by simon on 15.02.17.
//

#include <glm/geometric.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "RendererRayCasting.h"
#include "DataManagement/Particle.h"
#include "DataManagement/Timestep.h"
#include "Cameras/Camera.h"
#include "SkyBox.h"
#include <Shader/EffectShader.h>

RendererRayCasting::RendererRayCasting(GLfloat rayStepSize, SkyBox* skyBox) :
        _rayStepSize(rayStepSize),
        _skyBox(skyBox){
    _shader = new EffectShader("shader/raycasting.vert", "shader/raycasting.frag");
    _lightPos = glm::vec3(0.75f, 0.75f, -1.0f);
    srand(100);
}

RendererRayCasting::~RendererRayCasting() {}

void RendererRayCasting::setData(Timestep *step, uint32_t count) { // Timestep interpolated data
    glm::vec3 stepDimension = step->getParticleNumberPerDirection();
    //uint32_t symmetricSize = stepDimension.x * stepDimension.x * stepDimension.x;
    //Load buffer to 3D texture
    _particleCount = count;
    GLfloat* pData = new GLfloat[_particleCount*4];
    //GLubyte* pData = new GLubyte[particleCount];

    uint32_t index = 0;

    for (auto i = 0u; i < count; i++) {
        Particle tmp = step->getParticle(i);
        pData[index++] = 0.0f;
        pData[index++] = 0.0f;
        pData[index++] = 0.0f;
        pData[index++] = tmp.density * 500;   // each pData-value 0..255
        //pData[i] = step->getParticle(i).density >= 0.0f ? 255 : 0;
        //pData[i] = 1.0f;
    }

    //std::memset(&pData[index], 0.0f, (symmetricSize - particleCount) * 4);

    _texture = new Texture(stepDimension.x, stepDimension.y, stepDimension.z, THREE, GL_CLAMP_TO_BORDER, GL_LINEAR, "volume");
    _texture->setData(pData, GL_RGBA4, GL_RGBA);

    createShaderRandomValues();

    //Create Unit Cube and load it to GPU
    glGenVertexArrays(1, &_cubeVAO);
    glGenBuffers(1, &_cubeVBO);
    glGenBuffers(1, &_cubeEBO);
    glm::vec3 vertices[8]= {
            glm::vec3(0.0f,0.0f,0.0f),
            glm::vec3(1.0f,0.0f,0.0f),
            glm::vec3( 1.0f, 1.0f,0.0f),
            glm::vec3(0.0f, 1.0f,0.0f),

            glm::vec3(0.0f,0.0f, 1.0f),
            glm::vec3( 1.0f,0.0f, 1.0f),
            glm::vec3( 1.0f, 1.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 1.0f)
    };

    GLushort cubeIndices[36]={0,5,4, 5,0,1, 3,7,6, 3,6,2, 7,4,6, 6,4,5, 2,1,3, 3,1,0, 3,0,7, 7,0,4, 6,5,2, 2,5,1};

    glBindVertexArray(_cubeVAO);
    glBindBuffer (GL_ARRAY_BUFFER, _cubeVBO);
    glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), &(vertices[0].x), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, _cubeEBO);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), &cubeIndices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void RendererRayCasting::createShaderRandomValues() {
    GLfloat* randomValues = new GLfloat[800*600];
    GLfloat maxRandomValue = 0.0001f;
    for(auto i = 0u; i < 800 * 600; i++) {
        randomValues[i] = rand() / (GLfloat)RAND_MAX * maxRandomValue;
    }

    _randomTexture = new Texture(800, 600, 0, TWO, GL_CLAMP_TO_EDGE, GL_NEAREST, "randomValues");
    _randomTexture->setData(randomValues, GL_RED, GL_RED);
}

void RendererRayCasting::render(Camera *camera, WindowHandler *wHandler) {
    glEnable(GL_BLEND);

    glm::mat4 model = glm::mat4();
    glm::vec3 camPos = camera->Position;
    glm::vec3 step_size = glm::vec3(_rayStepSize);

    _shader->use();
    _skyBox->activate(_shader, "cubeTexture");
    _texture->activate(_shader);
    _randomTexture->activate(_shader);
    _shader->setModelViewProjection(model, camera, wHandler);
    glUniform3fv(glGetUniformLocation(_shader->Program, "camPos"), 1, glm::value_ptr(camPos));
    glUniform3fv(glGetUniformLocation(_shader->Program, "lightPos"), 1, glm::value_ptr(_lightPos));
    glUniform3fv(glGetUniformLocation(_shader->Program, "step_size"), 1, glm::value_ptr(step_size));
    glBindVertexArray(_cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

    glDisable(GL_BLEND);

    _shader->unUse();
}

void RendererRayCasting::changeStepSize(GLfloat value) {
    _rayStepSize += value;
}

void RendererRayCasting::changeLightPos() {
    _lightPos = (_lightPos == glm::vec3(0.25f, 0.75f, -1.0f)) ? glm::vec3(0.75f, 0.75f, -1.0f) : glm::vec3(0.25f, 0.75f, -1.0f);
}

void RendererRayCasting::toggleReflection() {
    _shader->use();
    _shader->toggleReflection();
    _shader->unUse();
}

void RendererRayCasting::toggleShadow() {
    _shader->use();
    _shader->toggleShadow();
    _shader->unUse();
}


