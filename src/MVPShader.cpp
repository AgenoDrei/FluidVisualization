#include "MVPShader.h"
#include "BaseCamera.h"
#include <glm/gtc/type_ptr.hpp>

MVPShader::MVPShader(const GLchar* vertexShader, const GLchar* fragmentShader)
        : Shader(vertexShader, fragmentShader) {

    _modelLocation = glGetUniformLocation(Program, "model");
    _viewLocation = glGetUniformLocation(Program, "view");
    _projectionLocation = glGetUniformLocation(Program, "projection");
}

void MVPShader::setModel(glm::mat4& model) {
    glUniformMatrix4fv(_modelLocation, 1, GL_FALSE, glm::value_ptr(model));
}

void MVPShader::setView(glm::mat4& view) {
    glUniformMatrix4fv(_viewLocation, 1, GL_FALSE, glm::value_ptr(view));
}

void MVPShader::setProjection(glm::mat4& projection) {
    glUniformMatrix4fv(_projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void MVPShader::setModelViewProjection(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    setModel(model);
    setView(view);
    setProjection(projection);
}

void MVPShader::setModelViewProjection(glm::mat4& model, BaseCamera* camera, WindowHandler* wHandler) {
    auto view = camera->GetViewMatrix();
    auto projection = camera->GetProjectonMatrix(wHandler, 0.5f, 10.0f);
    setModelViewProjection(model, view, projection);
}


