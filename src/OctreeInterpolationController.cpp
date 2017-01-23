//
// Created by simon on 22.01.17.
//

#include "OctreeInterpolationController.h"
#include "DataSet.h"
#include "OctreeNode.h"
#include "Timestep.h"


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

    //ToDo: Insert all data in insert List of root node
    Timestep* step = data->getTimestep(0);
    for(auto i = 0u; i < step->getSize(); i++) {
        root->addInsert(step->getParticle(i));
    }

    buildOctree(root);
}

void OctreeInterpolationController::compute() {

}

//Recursive function for octree
void OctreeInterpolationController::buildOctree(OctreeNode* node) {
    //Check Termination criteria: Only one or zero Particle in list
    if(node->getInsertListSize() == 1) {
        node->setExitNode(node->getInsertListElement(0));
        return;
    } else if(node->getInsertListSize() < 1) {
        return;
    }

    //Calculate mid of block for division
    GLfloat half = node->length / 2.0f;

    //Create 8 divisions in Block
    OctreeNode** nodes = new OctreeNode*[8] {0};
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
    for(auto i = 0u; i < node->getInsertListSize(); i++) {
        Particle currentParticle = node->getInsertListElement(i);
        for(auto j = 0u; j < 8; j++) {
            if(nodes[j]->isContained(currentParticle)) {
                nodes[j]->addInsert(currentParticle);
                break;
            }
        }
    }

    for(auto i = 0u; i < 8; i++) {
        buildOctree(nodes[i]);
    }
}
