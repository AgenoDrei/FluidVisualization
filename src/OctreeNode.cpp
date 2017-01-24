//
// Created by simon on 22.01.17.
//

#include "OctreeNode.h"

OctreeNode::OctreeNode(glm::vec3 anchor, float size) :
        exitNode(true),
        emptyNode(true),
        position(anchor),
        length(size){
    for(auto i = 0u; i < 8; i++) {
        childNodes[i] = nullptr;
    }
}

bool OctreeNode::isContained(glm::vec3 particle) {
    float minX = position.x;
    float maxX = position.x + length;
    float minY = position.y;
    float maxY = position.y + length;
    float minZ = position.z;
    float maxZ = position.z + length;
    if(particle.x >= minX && particle.x <= maxX
       && particle.y >= minY && particle.y <= maxY
            && particle.z >= minZ && particle.z <= maxZ) {
        return true;
    }
    return false;
}

void OctreeNode::setExitNode(Particle particle) {
    emptyNode = false;
    data = particle;
}

bool OctreeNode::isExitNode() {
    if(exitNode)
        return true;
    return false;
}

<<<<<<< HEAD
bool OctreeNode::isEmptyNode() {
    if(emptyNode)
        return true;
    return false;
}

=======
>>>>>>> 035e8c2e62578537e8dc52b291dd168dfd8c6c14
void OctreeNode::setChildNodes(OctreeNode **nodes) {
    exitNode = false;
    for(auto i = 0u; i < 8; i++) {
        childNodes[i] = nodes[i];
    }
}

OctreeNode* OctreeNode::getNode(uint32_t index) {
    return childNodes[index];
}

void OctreeNode::addInsert(Particle particle) {
    emptyNode = false;
    insertList.push_back(particle);
}

uint32_t OctreeNode::getInsertListSize() {
    return insertList.size();
}

Particle OctreeNode::getInsertListElement(uint32_t index) {
    return insertList.at(index);
}

Particle OctreeNode::getData() {
    return data;
}
