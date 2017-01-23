//
// Created by simon on 22.01.17.
//

#include "OctreeNode.h"

OctreeNode::OctreeNode(glm::vec3 anchor, float size) :
        exitNode(false),
        position(anchor),
        length(size){
    for(auto i = 0u; i < 8; i++) {
        childNodes[i] = nullptr;
    }
}

bool OctreeNode::isContained(Particle particle) {
    float minX = position.x;
    float maxX = position.x + length;
    float minY = position.y;
    float maxY = position.y + length;
    float minZ = position.z;
    float maxZ = position.z;
    if(particle.position.x >= minX && particle.position.x <= maxX
       && particle.position.y >= minY && particle.position.y <= maxY
            && particle.position.z >= minZ && particle.position.z <= maxZ) {
        return true;
    }
    return false;
}

void OctreeNode::setExitNode(Particle particle) {
    exitNode = true;
    data = particle;
}

void OctreeNode::setChildNodes(OctreeNode **nodes) {
    for(auto i = 0u; i < 8; i++) {
        childNodes[i] = nodes[i];
    }
}

OctreeNode* OctreeNode::getNode(uint32_t index) {
    return childNodes[index];
}

void OctreeNode::addInsert(Particle particle) {
    insertList.push_back(particle);
}

uint32_t OctreeNode::getInsertListSize() {
    return insertList.size();
}

Particle OctreeNode::getInsertListElement(uint32_t index) {
    return insertList.at(index);
}