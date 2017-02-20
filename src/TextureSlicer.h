#pragma once

#include <GL/glew.h>
#include <glm/ext.hpp>

class TextureSlicer {
public:
    TextureSlicer();
    ~TextureSlicer();
    void sliceVolumedata(glm::vec3 viewDir);
    void setNumSlices(int numSlices);
    glm::vec3* getSlicedVolume();
private:
    const float EPSILON = 0.0001f;
//    // xyz-coordinates for each UC-vertice
//    glm::vec3 ucVertices[8] = {
//            glm::vec3(0,0,0),
//            glm::vec3( 1,0,0),
//            glm::vec3(1, 1,0),
//            glm::vec3(0, 1,0),
//            glm::vec3(0,0, 1),
//            glm::vec3(1,0, 1),
//            glm::vec3( 1, 1, 1),
//            glm::vec3(0, 1, 1)
//    };
//    // between which vertices the UC-edges reside
//    const int ucEdgesPos[12][2]= {
//            {0,1},{1,2},{2,3},{3,0},
//            {0,4},{1,5},{2,6},{3,7},
//            {4,5},{5,6},{6,7},{7,4}
//    };
//    // There are only three unique paths when going from the nearest vertex to the farthest vertex from the camera
//    int possibleTraverses[8][12] = {
//            { 0,1,5,6,   4,8,11,9,  3,7,2,10 }, // v0 is front; v0 -> v6
//            { 0,4,3,11,  1,2,6,7,   5,9,8,10 }, // v1 is front; v1 -> v7
//            { 1,5,0,8,   2,3,7,4,   6,10,9,11}, // v2 is front: v2 -> v4
//            { 7,11,10,8, 2,6,1,9,   3,0,4,5  }, // v3 is front; v3 -> v5
//            { 8,5,9,1,   11,10,7,6, 4,3,0,2  }, // v4 is front; v4 -> v2
//            { 9,6,10,2,  8,11,4,7,  5,0,1,3  }, // v5 is front; v5 -> v3
//            { 9,8,5,4,   6,1,2,0,   10,7,11,3}, // v6 is front; v6 -> v0
//            { 10,9,6,5,  7,2,3,1,   11,4,8,0 }  // v7 is front; v7 -> v1
//    };
    int numSlices;
    glm::vec3* ucVertices;
    int* ucEdgesPos;
    int* possibleTraverses;
    glm::vec3* vTextureSlices;
    void calcMaxMinDistances(glm::vec3 viewDir,
                             float* minDist, float* maxDist, int* maxIdx);
    void calcVecsAndLambdas(glm::vec3 viewDir, float minDist, float maxDist, int maxIdx,
                            glm::vec3* vecStart[12], glm::vec3* vecDir[12], float* lambda[12], float* lambdaInc[12]);
    glm::vec3* calcTextureSlicesVolume(glm::vec3* vecStart[12], glm::vec3* vecDir[12], float* lambda[12], float* lambdaInc[12],
                                       glm::vec3* vTextureSlices);

};
