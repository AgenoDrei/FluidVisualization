#include "Renderer3DTextureSlicing.h"

#include "DataSet.h"
#include "Timestep.h"
#include "WindowHandler.h"
#include "Shader.h"

Renderer3DTextureSlicing::Renderer3DTextureSlicing() {
    shader = new Shader("shader/textureSlicer.vert", "shader/textureSlicer.frag");
}

Renderer3DTextureSlicing::~Renderer3DTextureSlicing() {
//    delete [] buffer;
}
void Renderer3DTextureSlicing::render(Timestep* step, uint32_t count, Camera* camera, WindowHandler* wHandler) {
    particleCount = count;
    dimX = dimY = dimZ = (uint32_t)std::pow(particleCount, 1/3.);   // assumption: exactly cubic !

    loadIntoTexture(step);
    bindVertex();

    sliceVolume();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO);
    shader.Use();
    glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vTextureSlices)/
                                  sizeof(vTextureSlices[0]));
    shader.UnUse();
    glDisable(GL_BLEND);
}

void Renderer3DTextureSlicing::loadIntoTexture(Timestep* step) {
    glm::vec3* pData = new glm::vec3[particleCount];

    for (auto i = 0u; i < particleCount; i++) {
        pData[i] = step->getParticle(i).position;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 4);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, dimX, dimY, dimZ, 0, GL_RED, GL_FLOAT_VEC3, pData);
    glGenerateMipmap(GL_TEXTURE_3D);
}

void Renderer3DTextureSlicing::bindVertex() {
    const int MAX_SLICES = 2 * dimZ;    // not sure though
    glm::vec3 vTextureSlices[MAX_SLICES*12];
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer (GL_ARRAY_BUFFER, VBO);
    glBufferData (GL_ARRAY_BUFFER, sizeof(vTextureSlices), 0, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0,0);
    glBindVertexArray(0);
}

void Renderer3DTextureSlicing::sliceVolume() {
//determine max and min distances
    glm::vec3 vecStart[12];
    glm::vec3 vecDir[12];
    float lambda[12];
    float lambda_inc[12];
    float denom = 0;
    float plane_dist = min_dist;
    float plane_dist_inc = (max_dist-min_dist)/float(num_slices);

//determine vecStart and vecDir values
    glm::vec3 intersection[6];
    float dL[12];
    for(int i=num_slices-1;i>=0;i--) {
        for(int e = 0; e < 12; e++)
        {
            dL[e] = lambda[e] + i*lambda_inc[e];
        }
        if
                ((dL[0] >= 0.0) && (dL[0] < 1.0))
            intersection[0] = vecStart[0] +
                              dL[0]*vecDir[0];
        {
        }
//like wise for all intersection points
        int indices[]={0,1,2, 0,2,3, 0,3,4, 0,4,5};
        for(int i=0;i<12;i++)
            vTextureSlices[count++]=intersection[indices[i]];
    }
//update buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vTextureSlices), &(vTextureSlices[0].x));
}