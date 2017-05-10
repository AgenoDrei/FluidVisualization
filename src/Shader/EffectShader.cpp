#include "MVPShader.h"
#include "Cameras/BaseCamera.h"
#include <glm/gtc/type_ptr.hpp>

#include "EffectShader.h"

EffectShader::EffectShader(const GLchar* vertexShader, const GLchar* fragmentShader)
        : MVPShader(vertexShader, fragmentShader) {
    _reflecting = 0;
    _shadowing = 0;
}

void EffectShader::toggleReflection() {
    _reflecting = !_reflecting;
    glUniform1f(glGetUniformLocation(Program, "reflection"), _reflecting);
}

void EffectShader::toggleShadow() {
    _shadowing = !_shadowing;
    glUniform1f(glGetUniformLocation(Program, "shadow"), _shadowing);
}
