#pragma once

#include "MVPShader.h"

class MarchingCubesShader : public MVPShader {
protected:
    GLint _reflectionViewLocation;
public:
    MarchingCubesShader();

    void setReflectionView(glm::mat4& view);

};



