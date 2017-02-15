#include <glm/gtc/type_ptr.hpp>
#include "MarchingCubesShader.h"

MarchingCubesShader::MarchingCubesShader()
    : MVPShader("shader/renderMarchingCubes.vert", "shader/renderMarchingCubes.frag") {

    _reflectionViewLocation = glGetUniformLocation(Program, "reflectionView");
}

void MarchingCubesShader::setReflectionView(glm::mat4& view) {
    glUniformMatrix4fv(_reflectionViewLocation, 1, GL_FALSE, glm::value_ptr(view));
}

