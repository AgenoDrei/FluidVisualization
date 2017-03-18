#include <glm/gtc/type_ptr.hpp>
#include "MarchingCubesShader.h"

MarchingCubesShader::MarchingCubesShader()
    : MVPShader("shader/renderMarchingCubes.vert", "shader/renderMarchingCubes.frag") {

    _reflectionViewLocation = glGetUniformLocation(Program, "reflectionView");
    _reflectionLocation = glGetUniformLocation(Program, "reflection");
}

void MarchingCubesShader::setReflectionView(glm::mat4& view) {
    glUniformMatrix4fv(_reflectionViewLocation, 1, GL_FALSE, glm::value_ptr(view));
}

void MarchingCubesShader::enableReflection() {
    GLfloat value = 1;
    glUniform1fv(_reflectionLocation, 1, &value);
}

void MarchingCubesShader::disableReflection() {
    GLfloat value = 0;
    glUniform1fv(_reflectionLocation, 1, &value);
}
