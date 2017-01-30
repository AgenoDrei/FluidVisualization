#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Camera;
class WindowHandler;

class MarchingCubesShader : public Shader {
private:
    GLint _modelLocation;
    GLint _viewLocation;
    GLint _projectionLocation;
public:
    MarchingCubesShader();

    void setModel(glm::mat4& model);
    void setView(glm::mat4& view);
    void setProjection(glm::mat4& projection);
    void setModelViewProjection(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void setModelViewProjection(glm::mat4& model, Camera* camera, WindowHandler* wHandler);
};



