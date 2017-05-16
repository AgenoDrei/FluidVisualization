#include <iostream>
#include <glm/gtc/random.inl>
#include "RendererDebugQuad.h"
#include "Shader/Shader.h"
#include "DataManagement/Particle.h"
#include "DataManagement/Timestep.h"

RendererDebugQuad::RendererDebugQuad() {
    shader = new Shader("shader/quad.vert", "shader/quad.frag");
}

RendererDebugQuad::~RendererDebugQuad() {
    delete [] buffer;
}

void RendererDebugQuad::setData(Timestep* step, uint32_t count) {
    std::cout << "Log> Size of float: " << sizeof(float) << std::endl;
    GLfloat quadData [] = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    buffer = new glm::vec4[count];
    uint32_t index = 0;
    for(auto j = 0u; j < count; j++) {
        Particle particle = step->getParticle(j);
        buffer[index] = glm::vec4(particle.position, particle.density * 500);
        index++;
    }

    uint32_t bufferElementSize = 5; // 3 pos floats, 2 tex Cords float
    verticeCount = 6;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, bufferElementSize * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, bufferElementSize * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //Texture Stuff
    if(count > 0) {
        glGenTextures(1, &texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, glm::sqrt(count), glm::sqrt(count), 0, GL_RGBA, GL_FLOAT, buffer);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

void RendererDebugQuad::render(BaseCamera *camera, WindowHandler *wHandler) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);

    shader->use();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, verticeCount);
    glBindVertexArray(0);

}

void RendererDebugQuad::render(BaseCamera *camera, WindowHandler *wHandler, GLuint textureId) {
    //glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glCullFace(GL_FRONT);

    shader->use();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, verticeCount);
    glBindVertexArray(0);

}