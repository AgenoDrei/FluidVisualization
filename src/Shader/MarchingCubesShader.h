#pragma once

#include "Shader/ReflectionShader.h"

class MarchingCubesShader : public ReflectionShader {
protected:
    GLint _reflectionViewLocation;
public:
    MarchingCubesShader();

    void setReflectionView(glm::mat4& view);
};



