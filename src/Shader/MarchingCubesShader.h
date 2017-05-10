#pragma once

#include "Shader/EffectShader.h"

class MarchingCubesShader : public EffectShader {
protected:
    GLint _reflectionViewLocation, _depthBiasMVPLocation;
public:
    MarchingCubesShader();
    void setReflectionView(glm::mat4& view);
    void setDepthBiasMVP(glm::mat4& depthBiasMVP);
};



