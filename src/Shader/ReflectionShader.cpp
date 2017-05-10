#include "MVPShader.h"
#include "Cameras/BaseCamera.h"
#include <glm/gtc/type_ptr.hpp>

#include "ReflectionShader.h"

ReflectionShader::ReflectionShader(const GLchar* vertexShader, const GLchar* fragmentShader)
        : MVPShader(vertexShader, fragmentShader) {
    _reflecting = 0;
    _reflectionLocation = glGetUniformLocation(Program, "reflection");
}

void ReflectionShader::toggleReflection() {
    if (!_reflecting) {
        _reflecting = 1;
        glUniform1fv(_reflectionLocation, 1, &_reflecting);
    } else {
        _reflecting = 0;
        glUniform1fv(_reflectionLocation, 1, &_reflecting);
    }
}
