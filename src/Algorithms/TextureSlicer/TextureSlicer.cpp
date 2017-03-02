#include <iostream>
#include "TextureSlicer.h"

TextureSlicer::TextureSlicer() {
    numSlices = 42;     // default is 42
    _vTextureSlices = new glm::vec3[numSlices*12];
//    _vTextureSlices = (glm::vec3*) malloc (sizeof(glm::vec3) * numSlices * 12);

    // xyz-coordinates for each UC-vertice
    _ucVertices = new glm::vec3[8] {
        glm::vec3(0,0,0),
        glm::vec3(1,0,0),
        glm::vec3(1,1,0),
        glm::vec3(0,1,0),
        glm::vec3(0,0,1),
        glm::vec3(1,0,1),
        glm::vec3(1,1,1),
        glm::vec3(0,1,1)
    };
    // between which vertices the UC-edges reside
    _ucEdgesPos = new int[12*2] {
        0,1, 1,2, 2,3, 3,0,
        0,4, 1,5, 2,6, 3,7,
        4,5, 5,6, 6,7, 7,4
    };
    // There are only three unique paths when going from the nearest vertex to the farthest vertex from the camera
    _possibleTraverses = new int[8*12] {
        0,1,5,6,   4,8,11,9,  3,7,2,10, // v0 is front; v0 -> v6
        0,4,3,11,  1,2,6,7,   5,9,8,10, // v1 is front; v1 -> v7
        1,5,0,8,   2,3,7,4,   6,10,9,11, // v2 is front: v2 -> v4
        7,11,10,8, 2,6,1,9,   3,0,4,5, // v3 is front; v3 -> v5
        8,5,9,1,   11,10,7,6, 4,3,0,2, // v4 is front; v4 -> v2
        9,6,10,2,  8,11,4,7,  5,0,1,3, // v5 is front; v5 -> v3
        9,8,5,4,   6,1,2,0,   10,7,11,3, // v6 is front; v6 -> v0
        10,9,6,5,  7,2,3,1,   11,4,8,0  // v7 is front; v7 -> v1
    };
}

TextureSlicer::~TextureSlicer() {
    delete [] _ucVertices;
    delete [] _ucEdgesPos;
    delete [] _possibleTraverses;
}


void TextureSlicer::setNumSlices(int numSlices) {
    int newNumSlices = numSlices;

    TextureSlicer::numSlices = newNumSlices;
    _vTextureSlices = (glm::vec3*) realloc (_vTextureSlices, sizeof(glm::vec3) * newNumSlices * 12);
}

glm::vec3* TextureSlicer::getSlicedVolume() {
    return _vTextureSlices;
}

void TextureSlicer::sliceVolumedata(glm::vec3 viewDir) {
    std::cout << "Slicing for viewDirSlicing: " << glm::to_string(viewDir) << std::endl;

    float minDist, maxDist;
    int maxIdx;
    glm::vec3 vecStart[12], vecDir[12];
    float lambda[12], lambdaInc[12];    // plane intersection parameter and -increment

    calcMaxMinDistances(viewDir, &minDist, &maxDist, &maxIdx);
    calcVecsAndLambdas(viewDir, minDist, maxDist, maxIdx, vecStart, vecDir, lambda, lambdaInc);
    fillTextureSlicesVolume(vecStart, vecDir, lambda, lambdaInc);
}


void TextureSlicer::calcMaxMinDistances(glm::vec3 viewDir,
                                        float* resMinDist, float* resMaxDist, int* maxIdx) {
    //get the max and min distance of each UC-vertex in the viewing direction
    float max_dist = glm::dot(viewDir, _ucVertices[0]);
    float min_dist = max_dist;
    int max_index = 0;
    for(int i=1;i<8;i++) {
        float dist = glm::dot(viewDir, _ucVertices[i]);
        if(dist > max_dist) {
            max_dist = dist;
            max_index = i;
        }
        if(dist<min_dist)
            min_dist = dist;
    }
    min_dist -= EPSILON;    //expand it a little bit
    max_dist += EPSILON;    //expand it a little bit

    *resMinDist = min_dist;
    *resMaxDist = max_dist;
    *maxIdx = max_index;
}

void TextureSlicer::calcVecsAndLambdas(glm::vec3 viewDir, float minDist, float maxDist, int maxIdx,
                                       glm::vec3 *vecStart, glm::vec3 *vecDir, float *lambda, float *lambdaInc) {
    float plane_dist = minDist,    //set the minimum distance as the plane_dist
            plane_dist_inc = (maxDist-minDist)/float(numSlices),
            denom;
//
//        vecStart[i] = _ucVertices[_ucEdgesPos[_possibleTraverses[max_index][i]][0]];     // get positionVector
//        vecDir[i] = _ucVertices[_ucEdgesPos[_possibleTraverses[max_index][i]][1]] - vecStart[i];   // get directionVector based from positionVector and accoridng traverse direction
//
    int* chosenTraverse = &(_possibleTraverses[maxIdx*12]);  //chosenTraverse is int[12] ...
    for(int i=0;i<12;i++) {     //for all edges
        int edgeIdx = chosenTraverse[i];
        int vStartIdx = _ucEdgesPos[edgeIdx*2];
        int vEndIdx = _ucEdgesPos[edgeIdx*2 + 1];
        vecStart[i] = _ucVertices[vStartIdx];
        vecDir[i] = _ucVertices[vEndIdx] - _ucVertices[vStartIdx];

        denom = glm::dot(vecDir[i], viewDir);       // combine traverseDirectionVetor with viewDirectionVector
        if (1.0 + denom != 1.0) {       // denom = 0 <=> vecDir _|_ viewDirSlicing
            lambda[i] = (plane_dist - glm::dot(vecStart[i],viewDir))/denom;
            lambdaInc[i] =  plane_dist_inc/denom;
        } else {
            lambda[i] = -1.0f;
            lambdaInc[i] =  0.0;
        }
    }
}

void TextureSlicer::fillTextureSlicesVolume(glm::vec3 *vecStart, glm::vec3 *vecDir, float *lambda, float *lambdaInc) {
    glm::vec3 intersection[6];  //note that for a plane and sub intersection, we can have a minimum of 3 and a maximum of 6 vertex polygon
    float dL[12];
    int count = 0;

    for(int i=numSlices-1;i>=0;i--) {  //loop through all slices
        for(int e = 0; e < 12; e++) // determine the lambda value for all edges
            dL[e] = lambda[e] + i*lambdaInc[e];

        //if the values are between 0-1, we have an intersection at the current edge; repeat the same for all 12 edges
        if  ((dL[0] >= 0.0) && (dL[0] < 1.0))	{
            intersection[0] = vecStart[0] + dL[0]*vecDir[0];
        }
        else if ((dL[1] >= 0.0) && (dL[1] < 1.0))	{
            intersection[0] = vecStart[1] + dL[1]*vecDir[1];
        }
        else if ((dL[3] >= 0.0) && (dL[3] < 1.0))	{
            intersection[0] = vecStart[3] + dL[3]*vecDir[3];
        } else
            continue;

        if ((dL[2] >= 0.0) && (dL[2] < 1.0))
            intersection[1] = vecStart[2] + dL[2]*vecDir[2];
        else if ((dL[0] >= 0.0) && (dL[0] < 1.0))
            intersection[1] = vecStart[0] + dL[0]*vecDir[0];
        else if ((dL[1] >= 0.0) && (dL[1] < 1.0))
            intersection[1] = vecStart[1] + dL[1]*vecDir[1];
        else
            intersection[1] = vecStart[3] + dL[3]*vecDir[3];

        if  ((dL[4] >= 0.0) && (dL[4] < 1.0))
            intersection[2] = vecStart[4] + dL[4]*vecDir[4];
        else if ((dL[5] >= 0.0) && (dL[5] < 1.0))
            intersection[2] = vecStart[5] + dL[5]*vecDir[5];
        else
            intersection[2] = vecStart[7] + dL[7]*vecDir[7];

        if	((dL[6] >= 0.0) && (dL[6] < 1.0))
            intersection[3] = vecStart[6] + dL[6]*vecDir[6];
        else if ((dL[4] >= 0.0) && (dL[4] < 1.0))
            intersection[3] = vecStart[4] + dL[4]*vecDir[4];
        else if ((dL[5] >= 0.0) && (dL[5] < 1.0))
            intersection[3] = vecStart[5] + dL[5]*vecDir[5];
        else
            intersection[3] = vecStart[7] + dL[7]*vecDir[7];

        if	((dL[8] >= 0.0) && (dL[8] < 1.0))
            intersection[4] = vecStart[8] + dL[8]*vecDir[8];
        else if ((dL[9] >= 0.0) && (dL[9] < 1.0))
            intersection[4] = vecStart[9] + dL[9]*vecDir[9];
        else
            intersection[4] = vecStart[11] + dL[11]*vecDir[11];

        if ((dL[10]>= 0.0) && (dL[10]< 1.0))
            intersection[5] = vecStart[10] + dL[10]*vecDir[10];
        else if ((dL[8] >= 0.0) && (dL[8] < 1.0))
            intersection[5] = vecStart[8] + dL[8]*vecDir[8];
        else if ((dL[9] >= 0.0) && (dL[9] < 1.0))
            intersection[5] = vecStart[9] + dL[9]*vecDir[9];
        else
            intersection[5] = vecStart[11] + dL[11]*vecDir[11];

        //after all 6 possible intersection vertices are obtained, we calculated the proper polygon indices by using indices of a triangular fan
        int indices[]={0,1,2, 0,2,3, 0,3,4, 0,4,5};
        //Using the indices, pass the intersection vertices to the _vTextureSlices vector
        for(int j=0;j<12;j++)
            _vTextureSlices[count++]=intersection[indices[j]];   // FILL TEXTURE SLICES VOLUME
    }
}
