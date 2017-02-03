#include "RendererMarchingCubes.h"
#include "Camera.h"
#include "VertexArrayBuffer.h"
#include "MarchingCubesShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include "MarchingCubesRenderObject.h"

RendererMarchingCubes::RendererMarchingCubes() {
    _shader = new MarchingCubesShader();
}

void RendererMarchingCubes::addTriangles(const std::vector<Triangle>& triangles) {
    auto vertexBuffer = new VertexArrayBuffer(static_cast<GLuint>(triangles.size() * 3));

    glBindVertexArray(vertexBuffer->getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * triangles.size(), &triangles[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))); // Normal attribute
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    _objects.push_back(new MarchingCubesRenderObject(vertexBuffer));
}

void RendererMarchingCubes::addVertexIndexBuffer(const std::vector<VertexPositionNormal>& vertices, const std::vector<int>& indices) {
    auto vertexBuffer = new VertexArrayBuffer(static_cast<GLuint>(vertices.size()));

    glBindVertexArray(vertexBuffer->getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionNormal) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))); // Normal attribute
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    _objects.push_back(new MarchingCubesRenderObject(indices.size(), indexBuffer, vertexBuffer));
}

void RendererMarchingCubes::render(Camera *camera, WindowHandler *wHandler) {
    glm::mat4 model = glm::mat4();
    _shader->use();
    _shader->setModelViewProjection(model, camera, wHandler);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_BLEND);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for(auto object : _objects) {
        glBindVertexArray(object->getVertexBuffer()->getVAO());
        if(object->isIndexed()) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->getIndexBuffer());

            glDrawElements(GL_TRIANGLES, object->getNumberIndices(), GL_UNSIGNED_INT, (void*)0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, object->getVertexBuffer()->getSize());
        }
        glBindVertexArray(0);
    }
}
