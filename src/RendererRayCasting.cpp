//
// Created by simon on 15.02.17.
//

#include <glm/geometric.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "RendererRayCasting.h"
#include "Particle.h"
#include "Timestep.h"
#include "Shader.h"
#include "Camera.h"

RendererRayCasting::RendererRayCasting() {
    shader = new Shader("shader/raycaster.vert", "shader/raycaster.frag");
}

RendererRayCasting::~RendererRayCasting() {

}

void RendererRayCasting::setData(Timestep *step, uint32_t count) { // Timestep interpolated data
    //Load buffer to 3D texture
    particleCount = count;
    GLfloat* pData = new GLfloat[particleCount];

    for (auto i = 0u; i < particleCount; i++) {
        pData[i] = step->getParticle(i).density;   // each pData-value 0..255
        //pData[i] = step->getParticle(i).density;   // each pData-value 0..255
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);      // GL_CLAMP_TO_BORDER instead of GL_CLAMP resolved GL_INVALID_ENUM error
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 4);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, 200, 100, 200, 0, GL_RED, GL_FLOAT, pData); //Remove magic numbers
    glGenerateMipmap(GL_TEXTURE_3D);

    //Create Unit Cube and load it to GPU
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);
    glm::vec3 vertices[8]= {
            glm::vec3(-0.5f,-0.5f,-0.5f),
            glm::vec3(0.5f,-0.5f,-0.5f),
            glm::vec3( 0.5f, 0.5f,-0.5f),
            glm::vec3(-0.5f, 0.5f,-0.5f),
            glm::vec3(-0.5f,-0.5f, 0.5f),
            glm::vec3( 0.5f,-0.5f, 0.5f),
            glm::vec3( 0.5f, 0.5f, 0.5f),
            glm::vec3(-0.5f, 0.5f, 0.5f)
    };

    GLushort cubeIndices[36]={0,5,4,5,0,1,3,7,6,3,6,2,7,4,6,6,4,5,2,1,3,3,1,0,3,0,7,7,0,4,6,5,2,2,5,1};

    glBindVertexArray(cubeVAO);
    glBindBuffer (GL_ARRAY_BUFFER, cubeVBO);
    glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), &(vertices[0].x), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), &cubeIndices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void RendererRayCasting::render(Camera *camera, WindowHandler *wHandler) {
    glEnable(GL_BLEND);

    shader->use();
    //glBindTexture(GL_TEXTURE_3D, texture);

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f));
    glm::mat4 view;
    view = camera->GetViewMatrix();
    glm::mat4 projection;
    projection = camera->GetProjectonMatrix(wHandler, 0.1f, 10.0f);
    glm::vec3 camPos = camera->Position;
    glm::vec3 step_size = glm::vec3(0.01f);

    // Pass the matrices to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(shader->Program, "camPos"), 1, glm::value_ptr(camPos));
    glUniform3fv(glGetUniformLocation(shader->Program, "step_size"), 1, glm::value_ptr(step_size));
    //glGetnUniformfv(glGetUniformLocation(shader->Program, "step_size"), 1, GL_FALSE, glm::value_ptr(step_size));

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 8);
    glBindVertexArray(0);

    glDisable(GL_BLEND);

}


