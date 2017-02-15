#include "Ground.h"
#include "VertexArrayBuffer.h"
#include "GroundShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

Ground::Ground() {
    _shader = new GroundShader();

    GLfloat vertices[] = {
            -1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    _vertexBuffer = new VertexArrayBuffer(4);
    glBindVertexArray(_vertexBuffer->getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer->getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))); // Texture attribute
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    unsigned int indices[] = {
            0, 1, 2,
            1, 3, 2
    };

    glGenBuffers(1, &_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Ground::render(Camera *camera, WindowHandler *wHandler) {
    glm::mat4 model = glm::mat4();
    _shader->use();
    _shader->setModelViewProjection(model, camera, wHandler);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_BLEND);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(_vertexBuffer->getVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

