#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "MVPShader.h"
#include "Shader.h"

class BaseCamera;
class WindowHandler;

class ReflectionShader : public MVPShader {
protected:
    GLint _reflectionLocation;
public:
    ReflectionShader(const GLchar* vertexShader, const GLchar* fragmentShader);

    void enableReflection();
    void disableReflection();
};