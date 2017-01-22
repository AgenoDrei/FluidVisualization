//
// Created by simon on 22.01.17.
//

#include "OctreeInterpolationController.h"
#include "DataSet.h"
#include "OctreeNode.h"


OctreeInterpolationController::OctreeInterpolationController() :
        interpolatedData(nullptr),
        sourceData(nullptr){}

OctreeInterpolationController::~OctreeInterpolationController() {
    delete [] interpolatedData;
}

DataSet* OctreeInterpolationController::interpolateData(DataSet *data) {
    prepareData(data);
    compute();
    return nullptr;
}

void OctreeInterpolationController::prepareData(DataSet *data) {
    glm::vec3 rootPos = glm::vec3(0.0f);
    GLfloat rootSize = 1.0f;
    root = new OctreeNode(rootPos, rootSize);
}

void OctreeInterpolationController::compute() {

}

//Recursive function for octree
void OctreeInterpolationController::buildOctree(OctreeNode* node, std::vector<Particle> *insertList) {
    //Check Termination criteria: Only one or zero Particle in list
    if(insertList->size() == 1) {
        node->setExitNode(insertList->front());
        return;
    } else if(insertList->size() < 1) {
        return;
    }

    //Calculate mid of block for division
    GLfloat half = node->length;

    //Create 8 divisions in Block
    OctreeNode* nodes[8];
    nodes[0] = new OctreeNode(node->position, half); //First section 0,0,0
    nodes[1] = new OctreeNode(glm::vec3(node->position.x + half, node->position.y, node->position.z), half); //Second 1,0,0
    nodes[2] = new OctreeNode(glm::vec3(node->position.x, node->position.y + half, node->position.z), half); //Third 0,1,0
    nodes[3] = new OctreeNode(glm::vec3(node->position.x + half, node->position.y + half, node->position.z), half); //Fourth 1,1,0

    nodes[4] = new OctreeNode(glm::vec3(node->position.x, node->position.y, node->position.z+half), half); //Fifth 0,0,1
    nodes[5] = new OctreeNode(glm::vec3(node->position.x + half, node->position.y, node->position.z+half), half); //Sixt 1,0,1
    nodes[6] = new OctreeNode(glm::vec3(node->position.x, node->position.y+half, node->position.z+half), half); //Seventh 0,1,1
    nodes[7] = new OctreeNode(glm::vec3(node->position.x + half, node->position.y + half, node->position.z + half), half); //Eight  1,1,1

    node->setChildNodes(nodes);

    //TODO: Split list for Nodes
}
