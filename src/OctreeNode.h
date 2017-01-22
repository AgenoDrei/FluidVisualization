//
// Created by simon on 22.01.17.
//

#pragma once


#include <glm/vec3.hpp>
#include "Particle.h"

class OctreeNode {
private:
    OctreeNode* childNodes[8];
    bool exitNode;
    Particle data;
public:
    OctreeNode(glm::vec3 anchor, float size);
    void setExitNode(Particle particle);
    void setChildNodes(OctreeNode** nodes);

    glm::vec3 position;
    float length;
};


