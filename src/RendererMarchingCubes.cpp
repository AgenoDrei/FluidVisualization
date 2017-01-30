#include "RendererMarchingCubes.h"
#include "Camera.h"
#include "VertexArrayBuffer.h"
#include "MarchingCubesShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

RendererMarchingCubes::RendererMarchingCubes() {
    _shader = new MarchingCubesShader();
}

void RendererMarchingCubes::addTriangles(const std::vector<Triangle>& triangles) {
    auto vertexBuffer = new VertexArrayBuffer(static_cast<GLuint>(triangles.size() * 3));

    glBindVertexArray(vertexBuffer->getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * triangles.size(), &triangles[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    _vertexBuffers.push_back(vertexBuffer);
}

void RendererMarchingCubes::render(Camera *camera, WindowHandler *wHandler) {
    glm::mat4 model = glm::mat4();
    _shader->use();
    _shader->setModelViewProjection(model, camera, wHandler);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for(auto vertexBuffer : _vertexBuffers) {
        glBindVertexArray(vertexBuffer->getVAO());
        glDrawArrays(GL_TRIANGLES, 0, vertexBuffer->getSize());
        glBindVertexArray(0);
    }
}
