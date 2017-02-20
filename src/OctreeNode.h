//
// Created by simon on 22.01.17.
//

#pragma once


#include <glm/vec3.hpp>
#include <vector>
#include "Particle.h"

class OctreeNode {
private:
    OctreeNode* childNodes[8];
    bool exitNode;
    bool emptyNode;
    std::vector<Particle> data;
    std::vector<Particle> insertList;
public:
    OctreeNode(glm::vec3 anchor, float size);
    void addData(Particle particle);
    bool isExitNode();
    bool isEmptyNode();
    void setChildNodes(OctreeNode** nodes);
    std::vector<Particle>* getData();
    OctreeNode* getNode(uint32_t index);
    void addInsert(Particle particle);
    uint32_t getInsertListSize();
    Particle getInsertListElement(uint32_t index);
    bool isContained(glm::vec3 particle);

    glm::vec3 position;
    float length;
};


