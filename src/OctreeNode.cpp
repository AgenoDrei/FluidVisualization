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

void OctreeNode::setExitNode(Particle particle) {
    exitNode = true;
    data = particle;
}

void OctreeNode::setChildNodes(OctreeNode **nodes) {
    for(auto i = 0u; i < 8; i++) {
        childNodes[i] = nodes[i];
    }
}