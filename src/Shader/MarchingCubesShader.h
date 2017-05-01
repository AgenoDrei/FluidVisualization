#pragma once

#include "Shader/ReflectionShader.h"

class MarchingCubesShader : public ReflectionShader {
protected:
    GLint _reflectionViewLocation, _depthBiasMVPLocation, _shadowLocation;
public:
    MarchingCubesShader();

    void setReflectionView(glm::mat4& view);

    void setDepthBiasMVP(glm::mat4& depthBiasMVP);

    void enableShadow();
    void disableShadow();
};



