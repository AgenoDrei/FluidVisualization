#include "RendererMarchingCubes.h"
#include "Cameras/Camera.h"
#include "VertexArrayBuffer.h"
#include "Shader/MarchingCubesShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <DataManagement/Timestep.h>
#include "Algorithms/MarchingCubes/MarchingCubesRenderObject.h"
#include "SkyBox.h"
#include "WindowHandler.h"
#include "TextureRenderer.h"
#include "Cameras/ReflectionCamera.h"
#include "Shader/ShadowMapShader.h"
#include "Renderer/RendererDebugQuad.h"

RendererMarchingCubes::RendererMarchingCubes(SkyBox* skyBox) :
    _skyBox(skyBox) {
    _shader = new MarchingCubesShader();
    _shadowShader = new ShadowMapShader();
    _debug = new RendererDebugQuad();
    _debug->setData(nullptr, 0);

    glGenFramebuffers(1, &_reflectionFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _reflectionFramebuffer);

    glGenTextures(1, &_reflectionTexture);
    glBindTexture(GL_TEXTURE_2D, _reflectionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenRenderbuffers(1, &_reflectionDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _reflectionDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 2048, 2048);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _reflectionDepthBuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _reflectionTexture, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw "fail";
    }


    glGenFramebuffers(1, &_shadowMapFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _shadowMapFramebuffer);

    glGenTextures(1, &_depthTexture);
    glBindTexture(GL_TEXTURE_2D, _depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,  _depthTexture, 0);

    glDrawBuffer(GL_NONE); // No color buffer is drawn to
    glReadBuffer(GL_NONE);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)  {
        throw "fail";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // reset framebuffer

    _debugRenderer = new TextureRenderer(_depthTexture);

    enableShadow();
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

void RendererMarchingCubes::clean() { // todo delete all stuff
    _objects.clear();
}

void RendererMarchingCubes::enableReflection() {
    _shader->use();
    _shader->enableReflection();
}

void RendererMarchingCubes::disableReflection() {
    _shader->use();
    _shader->disableReflection();
}

void RendererMarchingCubes::enableShadow() {
    _shader->use();
    _shader->enableShadow();
}

void RendererMarchingCubes::disableShadow() {
    _shader->use();
    _shader->disableShadow();
}

void RendererMarchingCubes::renderReflectionMap(BaseCamera *camera, WindowHandler *wHandler) {
    glBindFramebuffer(GL_FRAMEBUFFER, _reflectionFramebuffer);
    glViewport(0, 0, 2048, 2048);

    _skyBox->render(camera, wHandler);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, wHandler->getWidth(), wHandler->getHeight());
}

glm::mat4 RendererMarchingCubes::getDepthProjectionMatrix() {
    return glm::ortho<float>(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1.0);
}

void RendererMarchingCubes::renderShadowMap(BaseCamera *camera, WindowHandler *wHandler) {
    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, _shadowMapFramebuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT);

    //glm::vec3 lightInvDir = glm::vec3(0.3f,1.0,2);
    glm::vec3 lightInvDir = glm::vec3(0.0f, 0.76f, 1.0f);

    glm::mat4 depthProjectionMatrix = getDepthProjectionMatrix();
    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 depthModelMatrix = glm::mat4();

    _shadowShader->use();
    _shadowShader->setModelViewProjection(depthModelMatrix, depthViewMatrix, depthProjectionMatrix);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, wHandler->getWidth(), wHandler->getHeight());
}

void RendererMarchingCubes::render(BaseCamera *camera, WindowHandler *wHandler) {
    renderShadowMap(camera, wHandler);

    _debug->render(camera, wHandler, _depthTexture);

    //renderWithShadow(camera, wHandler);
}

void RendererMarchingCubes::renderWithShadow(BaseCamera *camera, WindowHandler *wHandler) {
    auto reflectionCamera = new ReflectionCamera(camera, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));

    //TODO: set clipping
    //renderReflectionMap(reflectionCamera, wHandler);

    /*_debugRenderer->render(camera, wHandler);
    return;*/

    glm::mat4 model = glm::mat4();
    _shader->use();
    _shader->setModelViewProjection(model, camera, wHandler);

    auto reflectionViewMatrix = reflectionCamera->GetViewMatrix();
    _shader->setReflectionView(reflectionViewMatrix);

    //glm::vec3 lightInvDir = glm::vec3(0.3f,1.0,2);
    glm::vec3 lightInvDir = glm::vec3(0.0f, 0.76f, 1.0f);

    glm::mat4 depthProjectionMatrix = getDepthProjectionMatrix();
    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;
    glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
    );
    //glm::mat4 depthBiasMVP = biasMatrix * depthMVP;
    glm::mat4 depthBiasMVP = depthMVP;

    _shader->setDepthBiasMVP(depthBiasMVP);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _reflectionTexture);
    //glBindTexture(GL_TEXTURE_2D, _depthTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _depthTexture);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
