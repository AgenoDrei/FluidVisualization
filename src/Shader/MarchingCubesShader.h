#pragma once

#include "Shader/ReflectionShader.h"

class MarchingCubesShader : public ReflectionShader {
protected:
    GLint _reflectionViewLocation, _depthBiasMVPLocation, _shadowLocation;
    GLfloat _shadowing;
public:
    MarchingCubesShader();

    void setReflectionView(glm::mat4& view);

    void setDepthBiasMVP(glm::mat4& depthBiasMVP);

    void toggleShadow();
    void enableShadow();
    void disableShadow();
};



