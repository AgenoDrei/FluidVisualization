//
// Created by simon on 22.01.17.
//

#include <iostream>
#include <glm/ext.hpp>
#include "OctreeInterpolationController.h"
#include "DataSet.h"
#include "OctreeNode.h"
#include "Timestep.h"


OctreeInterpolationController::OctreeInterpolationController(uint32_t quality, uint32_t depth) :
        interpolatedData(nullptr),
        sourceData(nullptr),
        resolution(quality),
        searchDepth(depth){}

OctreeInterpolationController::~OctreeInterpolationController() {
    delete [] interpolatedData;
}

DataSet* OctreeInterpolationController::interpolateData(DataSet *data) {
    prepareData(data);
    compute();
    return interpolatedData;
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
    //glm::vec3 test = glm::vec3(0.87f, 0.12f, 0.12f);
    //searchNode(test);

    uint32_t index = 0;
    uint32_t arraySize = resolution * resolution * resolution;
    Particle* grid = new Particle[arraySize];

    for(auto z = 0u; z < resolution; z++) {
        for(auto y = 0u; y < resolution; y++) {
            for(auto x = 0u; x < resolution; x++) {
                grid[index].reset();
                glm::vec3 position = glm::vec3(x / (float)resolution, y / (float)resolution, z / (float)resolution);
                grid[index].position = position;
                auto node = searchNode(position);
                if(node != nullptr) {
                    grid[index].density = node->getData().density;
                } else {
                    grid[index].density = 0;
                }
                index++;
            }
        }
    }

    auto timesteps = new Timestep*[1];
    timesteps[0] = new Timestep(grid, arraySize);
    auto result = new DataSet(arraySize, 1, timesteps);

    interpolatedData = result;
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

    for(auto i = 0u; i < node->getInsertListSize(); i++) {
        Particle currentParticle = node->getInsertListElement(i);
        for(auto j = 0u; j < 8; j++) {
            if(nodes[j]->isContained(currentParticle.position)) {
                nodes[j]->addInsert(currentParticle);
                break;
            }
        }
    }

    for(auto i = 0u; i < 8; i++) {
        buildOctree(nodes[i]);
    }
}

OctreeNode* OctreeInterpolationController::searchNode(glm::vec3 searchPosition) {
    OctreeNode* node = root;
    OctreeNode* parent = nullptr;
    uint32_t depth = 1;

    while(!node->isExitNode()) {
        for(auto i = 0u; i < 8; i++) {
            OctreeNode* child = node->getNode(i);
            if(child->isContained(searchPosition)) {
                parent = node;
                node = child;
                depth++;
                break;
            }
        }
    }

    if(depth >= searchDepth && node->isEmptyNode()) {
        while(!parent->isExitNode()) {
            for(auto i = 0u; i < 8; i++) {
                OctreeNode* child = parent->getNode(i);
                if(!child->isEmptyNode()) {
                    parent = child;
                    break;
                }
            }
        }
        return parent;
    } else if(depth >=7 && !node->isEmptyNode()) {
        return node;
    } else {
        return nullptr;
    }

}
