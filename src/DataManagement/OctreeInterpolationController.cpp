#include <iostream>
#include <glm/ext.hpp>
#include "OctreeInterpolationController.h"
#include "DataManagement/DataSet.h"
#include "OctreeNode.h"
#include "Timestep.h"

OctreeInterpolationController::OctreeInterpolationController(GLfloat minSize, GLfloat distanceCorrectionFactor) :
        interpolatedData(nullptr),
        sourceData(nullptr),
        _minSize(minSize),
        distanceCorrectionFactor(distanceCorrectionFactor){}

OctreeInterpolationController::~OctreeInterpolationController() {
    delete [] interpolatedData;
}

DataSet* OctreeInterpolationController::interpolateData(DataSet *data, GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ) {
    std::cout << "Log> Start building Octree..." << std::endl;
    prepareData(data);
    std::cout << "Log> Finished building Octree DONE" << std::endl;
    std::cout << "Log> Start interpolation..." << std::endl;
    compute(resolutionX, resolutionY, resolutionZ);
    std::cout << "Log> Finished interpolation DONE" << std::endl;
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

void OctreeInterpolationController::compute(GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ) {
    //get max(resolutionX, resolutionY, resolutionZ)
    uint32_t resolution = (uint32_t) (resolutionX > resolutionY ? ( resolutionX > resolutionZ ? resolutionX : resolutionZ ) : ( resolutionY > resolutionZ ? resolutionY : resolutionZ ));

    uint32_t index = 0;
    uint32_t arraySize = (uint32_t) (resolutionX * resolutionY * resolutionZ);
    Particle* grid = new Particle[arraySize];
    float maxDistance = 1.0f / resolutionX / distanceCorrectionFactor;
    //float maxDistance = 0.005f;

    for(auto z = 0u; z < resolutionZ; z++) {
        for(auto y = 0u; y < resolutionY; y++) {
            for(auto x = 0u; x < resolutionX; x++) {
                grid[index].reset();
                glm::vec3 position = glm::vec3(x / (float)resolution, y / (float)resolution, z / (float)resolution);
                grid[index].position = position;
                auto node = searchNode(position);
                if(node != nullptr) {
                    //glm::vec3 relevantPosition = node->getData().position;
                    std::vector<Particle>* candidates = node->getData();
                    GLfloat smallestDistance = FLT_MAX;
                    uint32_t smallestDistanceIndex = INFINITY;
                    for(auto i = 0u; i < candidates->size(); i++) {
                        Particle candidate = candidates->at(i);
                        float distance = glm::distance(position, candidate.position);
                        if(distance < smallestDistance) {
                            smallestDistance = distance;
                            smallestDistanceIndex = i;
                        }
                    }

                    if(smallestDistance <= maxDistance) {
                        grid[index].density = candidates->at(smallestDistanceIndex).density;
                    } else {
                        grid[index].density = 0;
                    }
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
        node->addData(node->getInsertListElement(0));
        return;
    } else if(node->length <= _minSize) {
        for(auto i = 0u; i < node->getInsertListSize(); i++) {
            node->addData(node->getInsertListElement(i));
        }
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

    if(!node->isEmptyNode())
        return node;
    else if (depth > 8)
        return parent;
    else
        return nullptr;
}
