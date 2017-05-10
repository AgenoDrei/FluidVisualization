#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glm/ext.hpp>
#include "MarchingCubesShader.h"

MarchingCubesShader::MarchingCubesShader()
    : ReflectionShader("shader/renderMarchingCubes.vert", "shader/renderMarchingCubes.frag") {

    use();

    _reflectionViewLocation = glGetUniformLocation(Program, "reflectionView");
    _depthBiasMVPLocation = glGetUniformLocation(Program, "depthBiasMVP");

    auto texLoc = glGetUniformLocation(Program, "ourTexture");
    glUniform1i(texLoc, 1);

    texLoc = glGetUniformLocation(Program, "shadowMap");
    glUniform1i(texLoc, 2);

    _shadowLocation = glGetUniformLocation(Program, "shadow");
    _shadowing = 0;
}

void MarchingCubesShader::setReflectionView(glm::mat4& view) {
    glUniformMatrix4fv(_reflectionViewLocation, 1, GL_FALSE, glm::value_ptr(view));
}

void MarchingCubesShader::setDepthBiasMVP(glm::mat4& depthBiasMVP) {
    glUniformMatrix4fv(_depthBiasMVPLocation, 1, GL_FALSE, glm::value_ptr(depthBiasMVP));
}

void MarchingCubesShader::toggleShadow() {
    if (!_shadowing) {
        _shadowing = 1;
        glUniform1fv(_shadowLocation, 1, &_shadowing);
    } else {
        _shadowing = 0;
        glUniform1fv(_shadowLocation, 1, &_shadowing);
    }
}
