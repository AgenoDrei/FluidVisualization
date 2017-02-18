#include <iostream>
#include <fstream>
#include "Renderer3DTextureSlicing.h"

#include "DataSet.h"
#include "Timestep.h"
#include "WindowHandler.h"
#include "Shader.h"

Renderer3DTextureSlicing::Renderer3DTextureSlicing(uint32_t dimX, uint32_t dimY, uint32_t dimZ) {
    shader = new Shader("shader/textureSlicer.vert", "shader/textureSlicer.frag");
    bViewRotated = false;
    Renderer3DTextureSlicing::num_slices = 1*dimZ;
    Renderer3DTextureSlicing::sizeof_vTextureSlices = num_slices*12*sizeof(glm::vec3);
    Renderer3DTextureSlicing::dimX = dimX;
    Renderer3DTextureSlicing::dimY = dimY;
    Renderer3DTextureSlicing::dimZ = dimZ;
}

Renderer3DTextureSlicing::~Renderer3DTextureSlicing() {}

void Renderer3DTextureSlicing::setData(Timestep* step, glm::vec3 initViewDir) {
    auto particleCount = step->getSize();
    auto pData = new float[particleCount];
    for (auto i = 0u; i < particleCount; i++) {
        pData[i] = step->getParticle(i).density * 1000;   // *1000 s.t. values big enough to get integer red value
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);      // GL_CLAMP_TO_BORDER instead of GL_CLAMP resolved GL_INVALID_ENUM error
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 4);
//    glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, dimX, dimY, dimZ, 0, GL_RED, GL_UNSIGNED_BYTE, pData);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, dimX, dimY, dimZ, 0, GL_RED, GL_FLOAT, pData);
    glGenerateMipmap(GL_TEXTURE_3D);

    delete [] pData;

    vTextureSlices = new glm::vec3[num_slices*12];

    //setup the vertex array and buffer objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vTextureSlices), 0, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof_vTextureSlices, 0, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);       //enable vertex attribute array for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);

    Renderer3DTextureSlicing::sliceVolume(initViewDir);     // initial slicing
}

void Renderer3DTextureSlicing::render(Camera* camera, WindowHandler* wHandler) {
    glm::mat4 model, view, projection;
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    view = camera->GetViewMatrix();
    projection = camera->GetProjectonMatrix(wHandler, 0.1f, 10.0f);

//    if (bViewRotated)
//        sliceVolume(camera->Front);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(VAO);
    shader->use();
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//    glUniform1i(glGetUniformLocation(shader->Program, "volume"), 0);
    //glDrawArrays(GL_TRIANGLES, 0, sizeof(vTextureSlices)/sizeof(vTextureSlices[0]));
    glDrawArrays(GL_TRIANGLES, 0, sizeof_vTextureSlices / sizeof(glm::vec3));
    shader->unUse();
}

void Renderer3DTextureSlicing::sliceVolume(glm::vec3 viewDir) {      // main slicing function
    std::cout << "ViewDir@slicing = " << glm::to_string(viewDir) << std::endl;
    const float EPSILON = 0.0001f;      //for floating point inaccuracy

    // xyz-coordinates for each UC-vertice
    glm::vec3 ucVertices[8] = {
        glm::vec3(0,0,0),
        glm::vec3( 0.9,0,0),
        glm::vec3(0.9, 0.9,0),
        glm::vec3(0, 0.9,0),
        glm::vec3(0,0, 0.9),
        glm::vec3(0.9,0, 0.9),
        glm::vec3( 0.9, 0.9, 0.9),
        glm::vec3(0, 0.9, 0.9)
    };
    // between which vertices the UC-edges reside
    const int ucEdgesPos[12][2]= {
            {0,1},{1,2},{2,3},{3,0},
            {0,4},{1,5},{2,6},{3,7},
            {4,5},{5,6},{6,7},{7,4}
    };
    // There are only three unique paths when going from the nearest vertex to the farthest vertex from the camera
    int possibleTraverses[8][12] = {
        { 0,1,5,6,   4,8,11,9,  3,7,2,10 }, // v0 is front; v0 -> v6
        { 0,4,3,11,  1,2,6,7,   5,9,8,10 }, // v1 is front; v1 -> v7
        { 1,5,0,8,   2,3,7,4,   6,10,9,11}, // v2 is front: v2 -> v4
        { 7,11,10,8, 2,6,1,9,   3,0,4,5  }, // v3 is front; v3 -> v5
        { 8,5,9,1,   11,10,7,6, 4,3,0,2  }, // v4 is front; v4 -> v2
        { 9,6,10,2,  8,11,4,7,  5,0,1,3  }, // v5 is front; v5 -> v3
        { 9,8,5,4,   6,1,2,0,   10,7,11,3}, // v6 is front; v6 -> v0
        { 10,9,6,5,  7,2,3,1,   11,4,8,0 }  // v7 is front; v7 -> v1
    };

    //get the max and min distance of each UC-vertex in the viewing direction
    float max_dist = glm::dot(viewDir, ucVertices[0]);
    float min_dist = max_dist;
    int max_index = 0;
    for(int i=1;i<8;i++) {
        float dist = glm::dot(viewDir, ucVertices[i]);
        if(dist > max_dist) {
            max_dist = dist;
            max_index = i;
        }
        if(dist<min_dist)
            min_dist = dist;
    }
    min_dist -= EPSILON;    //expand it a little bit
    max_dist += EPSILON;    //expand it a little bit

    glm::vec3 vecStart[12], vecDir[12];
    float lambda[12], lambda_inc[12], denom;   // plane intersection parameter (lambda) and -increment (lambda_inc)
    float plane_dist = min_dist;    //set the minimum distance as the plane_dist
    float plane_dist_inc = (max_dist-min_dist)/float(num_slices);

    for(int i=0;i<12;i++) {     //for all edges
        vecStart[i] = ucVertices[ucEdgesPos[possibleTraverses[max_index][i]][0]];     // get positionVector
        vecDir[i] = ucVertices[ucEdgesPos[possibleTraverses[max_index][i]][1]] - vecStart[i];   // get directionVector based from positionVector and accoridng traverse direction

        denom = glm::dot(vecDir[i], viewDir);       // combine traverseDirectionVetor with viewDirectionVector
        if (1.0 + denom != 1.0) {       // denom = 0 <=> vecDir _|_ viewDir
            lambda_inc[i] =  plane_dist_inc/denom;
            lambda[i]     = (plane_dist - glm::dot(vecStart[i],viewDir))/denom;
        } else {
            lambda[i]     = -1.0;
            lambda_inc[i] =  0.0;
        }
    }

    intersection = new glm::vec3[6];      //note that for a plane and sub intersection, we can have a minimum of 3 and a maximum of 6 vertex polygon
    float dL[12];
    int count = 0;
    for(int i=num_slices-1;i>=0;i--) {  //loop through all slices
        for(int e = 0; e < 12; e++) // determine the lambda value for all edges
            dL[e] = lambda[e] + i*lambda_inc[e];

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
        //Using the indices, pass the intersection vertices to the vTextureSlices vector
        for(int j=0;j<12;j++)
            vTextureSlices[count++]=intersection[indices[j]];
    }

    //update buffer object with the new vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof_vTextureSlices, &(vTextureSlices[0].x));
}
