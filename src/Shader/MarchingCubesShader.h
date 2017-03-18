#pragma once

#include "Shader/MVPShader.h"

class MarchingCubesShader : public MVPShader {
protected:
    GLint _reflectionViewLocation;
    GLint _reflectionLocation;
public:
    MarchingCubesShader();

    void setReflectionView(glm::mat4& view);

    void enableReflection();
    void disableReflection();
};



