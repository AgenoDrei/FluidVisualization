#pragma once

#include "Shader/ReflectionShader.h"

class MarchingCubesShader : public ReflectionShader {
protected:
    GLint _reflectionViewLocation, _depthBiasMVPLocation;
public:
    MarchingCubesShader();

    void setReflectionView(glm::mat4& view);

    void setDepthBiasMVP(glm::mat4& depthBiasMVP);
};



