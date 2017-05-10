#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "MVPShader.h"
#include "Shader.h"

class BaseCamera;
class WindowHandler;

class EffectShader : public MVPShader {
protected:
    bool _reflecting, _shadowing;
public:
    EffectShader(const GLchar* vertexShader, const GLchar* fragmentShader);
    void toggleReflection();
    void toggleShadow();
};