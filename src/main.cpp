#include "main.h"
#include "DataImporter.h"
#include "DataSet.h"
#include "WindowHandler.h"
#include "Camera.h"
#include "Shader.h"
#include "Timestep.h"
#include "DataInterpolator.h"
#include "InterpolationController.h"
#include "CpuInterpolationController.h"
#include "GpuInterpolationController.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>


WindowHandler* window;
Camera* camera;
GLuint VBO, VAO;
Shader* ourShader;
DataSet* data = nullptr, *interpolatedData = nullptr;
InterpolationController *ctrl;

GLuint texture;

int main(int argc, char* argv[]) {
    std::string path = std::getenv("HOME"); //weird clion bug, not important for compiling
	data = DataImporter::load(path + "/Downloads/drop.dat");
    ctrl = new CpuInterpolationController(20);
	//data = DataImporter::load("/home/nils/Downloads/drop.dat");

    //Window Initialisation
    window = new WindowHandler(800, 600);
    window->initWindow(argc, argv, &init, &mainLoop);
    return 0;
}

void init() {
    std::cout << "Log> Render initialization running" << std::endl;
    camera = new Camera(glm::vec3(0.5f, 0.4f, 1.7f));
    window->setCamera(camera);
    ctrl->createShader();
    ctrl->prepareGpuBuffer(data, 0);
    ctrl->loadGpuBuffer();
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);


    glPointSize(2);
    std::cout << "Log> Initalization done" << std::endl;
}

void mainLoop() {
    doMovement(camera, window);
    window->calculateFPS();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ctrl->renderParticles(camera, window);

    glutSwapBuffers();
}

// Moves/alters the camera positions based on user input
void doMovement(Camera* camera, WindowHandler* wHandler) {
	if(wHandler->getKey('w')) {
		camera->ProcessKeyboard(FORWARD, static_cast<float>(wHandler->getDeltaTime()));
	}
	if(wHandler->getKey('s')) {
		camera->ProcessKeyboard(BACKWARD, static_cast<float>(wHandler->getDeltaTime()));
	}
	if(wHandler->getKey('a')) {
		camera->ProcessKeyboard(LEFT, static_cast<float>(wHandler->getDeltaTime()));
	}
	if(wHandler->getKey('d')) {
		camera->ProcessKeyboard(RIGHT, static_cast<float>(wHandler->getDeltaTime()));
	}
    if(wHandler->getKey('n')) {
        camera->ProcessKeyboard(UP, static_cast<float>(wHandler->getDeltaTime()));
    }
    if(wHandler->getKey('m')) {
        camera->ProcessKeyboard(DOWN, static_cast<float>(wHandler->getDeltaTime()));
    }

}

