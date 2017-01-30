#include <glm/gtc/type_ptr.hpp>
#include "MarchingCubesShader.h"
#include "Camera.h"

MarchingCubesShader::MarchingCubesShader()
    : Shader("shader/renderMarchingCubes.vert", "shader/renderMarchingCubes.frag") {

    _modelLocation = glGetUniformLocation(Program, "model");
    _viewLocation = glGetUniformLocation(Program, "view");
    _projectionLocation = glGetUniformLocation(Program, "projection");
}

void MarchingCubesShader::setModel(glm::mat4& model) {
    glUniformMatrix4fv(_modelLocation, 1, GL_FALSE, glm::value_ptr(model));
}

void MarchingCubesShader::setView(glm::mat4& view) {
    glUniformMatrix4fv(_viewLocation, 1, GL_FALSE, glm::value_ptr(view));
}

void MarchingCubesShader::setProjection(glm::mat4& projection) {
    glUniformMatrix4fv(_projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void MarchingCubesShader::setModelViewProjection(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    setModel(model);
    setView(view);
    setProjection(projection);
}

void MarchingCubesShader::setModelViewProjection(glm::mat4& model, Camera* camera, WindowHandler* wHandler) {
    auto view = camera->GetViewMatrix();
    auto projection = camera->GetProjectonMatrix(wHandler, 0.1f, 10.0f);
    setModelViewProjection(model, view, projection);
}
