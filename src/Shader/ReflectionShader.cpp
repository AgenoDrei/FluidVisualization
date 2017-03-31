#include "MVPShader.h"
#include "Cameras/BaseCamera.h"
#include <glm/gtc/type_ptr.hpp>

#include "ReflectionShader.h"

ReflectionShader::ReflectionShader(const GLchar* vertexShader, const GLchar* fragmentShader)
        : MVPShader(vertexShader, fragmentShader) {

    _reflectionLocation = glGetUniformLocation(Program, "reflection");
}

void ReflectionShader::enableReflection() {
    GLfloat value = 1;
    glUniform1fv(_reflectionLocation, 1, &value);
}

void ReflectionShader::disableReflection() {
    GLfloat value = 0;
    glUniform1fv(_reflectionLocation, 1, &value);
}

