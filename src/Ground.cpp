#include "Ground.h"
#include "VertexArrayBuffer.h"
#include "GroundShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <SOIL/SOIL.h>

Ground::Ground() {
    _shader = new GroundShader();

    GLfloat vertices[] = { // Todo: VertexPositionTexture class
            -1000.0f, 0.0f, -1000.0f, 0.0f, 0.0f,
            1000.0f, 0.0f, -1000.0f, 1000.0f, 0.0f,
            -1000.0f, 0.0f, 1000.0f, 0.0f, 1000.0f,
            1000.0f, 0.0f, 1000.0f, 1000.0f, 1000.0f
    };

    _vertexBuffer = new VertexArrayBuffer(4);
    glBindVertexArray(_vertexBuffer->getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer->getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))); // Texture attribute
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    unsigned int indices[] = {
            0, 1, 2,
            1, 3, 2
    };

    glGenBuffers(1, &_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


    int width, height;
    auto image  = SOIL_load_image("textures/ground.jpeg", &width, &height, 0, SOIL_LOAD_RGBA);
    if(!image) {
        throw "can't load texture";
    }

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Ground::render(Camera *camera, WindowHandler *wHandler) {
    glm::mat4 model = glm::mat4();
    _shader->use();
    _shader->setModelViewProjection(model, camera, wHandler);

    glBindTexture(GL_TEXTURE_2D, _texture);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    glEnable(GL_BLEND);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindVertexArray(_vertexBuffer->getVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

