#include <glm/gtc/type_ptr.hpp>
#include "MarchingCubesShader.h"

MarchingCubesShader::MarchingCubesShader()
    : ReflectionShader("shader/renderMarchingCubes.vert", "shader/renderMarchingCubes.frag") {

    _reflectionViewLocation = glGetUniformLocation(Program, "reflectionView");
    _depthBiasMVPLocation = glGetUniformLocation(Program, "depthBiasMVP");

    auto texLoc = glGetUniformLocation(Program, "ourTexture");
    glUniform1i(texLoc, 0);

    texLoc = glGetUniformLocation(Program, "shadowMap");
    glUniform1i(texLoc, 1);
}

void MarchingCubesShader::setReflectionView(glm::mat4& view) {
    glUniformMatrix4fv(_reflectionViewLocation, 1, GL_FALSE, glm::value_ptr(view));
}

void MarchingCubesShader::setDepthBiasMVP(glm::mat4& depthBiasMVP) {
    glUniformMatrix4fv(_depthBiasMVPLocation, 1, GL_FALSE, glm::value_ptr(depthBiasMVP));
}
