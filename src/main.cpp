#include "main.h"
#include "DataImporter.h"
#include "DataSet.h"
#include "WindowHandler.h"
#include "Shader.h"
#include "Timestep.h"
#include "InterpolationController.h"
#include "CpuInterpolationController.h"
#include "GpuInterpolationController.h"
#include "RendererParticles.h"
#include "RendererDebugQuad.h"
#include "TextRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

WindowHandler* window;
TextRenderer* fpsRenderer;
Camera* camera;
DataSet* data = nullptr, *interpolatedData = nullptr;
InterpolationController *ctrl;
RendererParticles* renderer;
RendererDebugQuad* quadRenderer;

int main(int argc, char* argv[]) {
    std::string path = std::getenv("HOME"); //weird clion bug, not important for compiling
	data = DataImporter::load(path + "/Downloads/drop.dat");

    //Window Initialisation
    window = new WindowHandler(800, 600);
    window->initWindow(argc, argv, &init, &mainLoop);
    return 0;
}

void init() {
    std::cout << "Log> Render initialization running" << std::endl;
    camera = new Camera(glm::vec3(0.5f, 0.4f, 1.7f));
    window->setCamera(camera);
    ctrl = new CpuInterpolationController(10);
    renderer = new RendererParticles();
    quadRenderer = new RendererDebugQuad();

    interpolatedData = ctrl->interpolateData(data);
    renderer->setData(interpolatedData->getTimestep(0), interpolatedData->getNumberParticles());
    //quadRenderer->setData(data->getTimestep(0), data->getNumberParticles());
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fpsRenderer = new TextRenderer("../fonts/arial.ttf");

    glPointSize(2);
    std::cout << "Log> Initalization done" << std::endl;
}

void mainLoop() {
    doMovement(camera, window);
    std::string fps = std::to_string(window->calculateFPS());     // do not comment out, otherwise rendering stops after camera movement!?
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer->render(camera, window);
    //quadRenderer->render(camera, window);     // somehow not working anymore; guess is shared rendering ..
    fpsRenderer->drawText(fps, 21.0f, 21.0f, 1.0f, glm::vec3(0.3f, 0.7f, 0.9f));

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

