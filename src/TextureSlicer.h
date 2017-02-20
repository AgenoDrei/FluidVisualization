#pragma once

#include <GL/glew.h>
#include <glm/ext.hpp>

class TextureSlicer {
public:
    TextureSlicer();
    ~TextureSlicer();
    void setNumSlices(int numSlices);
    glm::vec3* getSlicedVolume();
    void sliceVolumedata(glm::vec3 viewDir);
    int numSlices;
private:
    const float EPSILON = 0.0001f;
    glm::vec3* ucVertices;
    int* ucEdgesPos;
    int* possibleTraverses;
    glm::vec3* vTextureSlices;
    void calcMaxMinDistances(glm::vec3 viewDir,
                             float* minDist, float* maxDist, int* maxIdx);
    void calcVecsAndLambdas(glm::vec3 viewDir, float minDist, float maxDist, int maxIdx,
                            glm::vec3 vecStart[12], glm::vec3 vecDir[12], float lambda[12], float lambdaInc[12]);
    void fillTextureSlicesVolume(glm::vec3 vecStart[12], glm::vec3 vecDir[12], float lambda[12], float lambdaInc[12]);

};
