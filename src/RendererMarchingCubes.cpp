#include "RendererMarchingCubes.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

VertexArrayBuffer::VertexArrayBuffer() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

RendererMarchingCubes::RendererMarchingCubes() {
    _shader = new Shader("shader/renderMarchingCubes.vert", "shader/renderMarchingCubes.frag"); // todo: own shader class
}

void RendererMarchingCubes::addTriangles(const std::vector<Triangle>& triangles) {
    VertexArrayBuffer vertexBuffer;
    vertexBuffer.numberOfVertices = static_cast<GLuint>(triangles.size() * 3);

    glBindVertexArray(vertexBuffer.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * triangles.size(), &triangles[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    _vertexBuffers.push_back(vertexBuffer);
}

void RendererMarchingCubes::render(Camera *camera, WindowHandler *wHandler) {
    _shader->use();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f));
    glm::mat4 view;
    model = camera->GetViewMatrix();
    glm::mat4 projection;
    projection = camera->GetProjectonMatrix(wHandler, 0.1f, 10.0f);

    auto modelLocation = glGetUniformLocation(_shader->Program, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(_shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(_shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    for(auto vertexBuffer : _vertexBuffers) {
        glBindVertexArray(vertexBuffer.VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexBuffer.numberOfVertices);
        glBindVertexArray(0);
    }
}
