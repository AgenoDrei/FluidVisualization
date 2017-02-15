#include "SkyBox.h"
#include "VertexArrayBuffer.h"
#include "SkyBoxShader.h"
#include <SOIL/SOIL.h>
#include <glm/detail/type_mat.hpp>

SkyBox::SkyBox() {
    _shader = new SkyBoxShader();

    std::vector<const GLchar*> faces;
    faces.push_back("textures/utopiaatoll_rt.jpeg");
    faces.push_back("textures/utopiaatoll_lf.jpeg");
    faces.push_back("textures/utopiaatoll_up.jpeg");
    faces.push_back("textures/utopiaatoll_dn.jpeg");
    faces.push_back("textures/utopiaatoll_bk.jpeg");
    faces.push_back("textures/utopiaatoll_ft.jpeg");

    loadSkyBox(faces);

    auto size = 4.0f;

    GLfloat vertices[] = {
            // Positions
            -size,  size, -size,
            -size, -size, -size,
            size, -size, -size,
            size, -size, -size,
            size,  size, -size,
            -size,  size, -size,

            // left
            -size, -size,  size,
            -size, -size, -size,
            -size,  size, -size,
            -size,  size, -size,
            -size,  size,  size,
            -size, -size,  size,

            //right
            size, -size, -size,
            size, -size,  size,
            size,  size,  size,
            size,  size,  size,
            size,  size, -size,
            size, -size, -size,

            //back
            -size, -size,  size,
            -size,  size,  size,
            size,  size,  size,
            size,  size,  size,
            size, -size,  size,
            -size, -size,  size,

            // top
            -size,  size, -size,
            size,  size, -size,
            size,  size,  size,
            -size,  size, -size,
            size,  size,  size,
            -size,  size,  size,

            -size, -size, -size,
            -size, -size,  size,
            size, -size, -size,
            size, -size, -size,
            -size, -size,  size,
            size, -size,  size
    };

    _vertexBuffer = new VertexArrayBuffer(6 * 6);
    glBindVertexArray(_vertexBuffer->getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer->getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void SkyBox::loadSkyBox(std::vector<const GLchar*> faces) {
    glGenTextures(1, &_texture);
    glActiveTexture(GL_TEXTURE0);

    int width,height;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
    for(GLuint i = 0; i < faces.size(); i++)  {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        if(!image) {
            throw "can't load face";
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,  GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyBox::render(Camera *camera, WindowHandler *wHandler) {
    glm::mat4 model = glm::mat4();
    render(camera, wHandler, model);
}

void SkyBox::render(Camera *camera, WindowHandler *wHandler, glm::mat4x4 model) {
    _shader->use();
    _shader->setModelViewProjection(model, camera, wHandler);

    glDepthMask(GL_FALSE);
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glBindVertexArray(_vertexBuffer->getVAO());
    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}
