#include "main.h"
#include "DataImporter.h"
#include "DataExporter.h"
#include "DataSet.h"
#include "WindowHandler.h"
#include "Timestep.h"
#include "InterpolationController.h"
#include "CpuInterpolationController.h"
#include "GpuInterpolationController.h"
#include "RendererParticles.h"
#include "RendererDebugQuad.h"
#include "Renderer3DTextureSlicing.h"
#include "TextRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

WindowHandler* window;
TextRenderer* fpsRenderer;
Camera* camera;
DataSet* data = nullptr;
Renderer3DTextureSlicing* renderer;

int main(int argc, char* argv[]) {
    std::string path = std::getenv("HOME");
    data = DataImporter::load(path + "/Downloads/drop_100.dat");

    //Window Initialisation
    window = new WindowHandler(800, 600);
    window->initWindow(argc, argv, &init, &mainLoop);
    return 0;
}

void init() {
    std::cout << "Log> Render initialization running" << std::endl;

    camera = new Camera(glm::vec3(0.5f, 0.4f, 1.7f));
    window->setCamera(camera);
    fpsRenderer = new TextRenderer("../fonts/arial.ttf");

    renderer = new Renderer3DTextureSlicing(4, 100, 100, 100, camera);
    renderer->setData(data->getTimestep(0));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    std::cout << "Log> Initalization done" << std::endl;
}

void mainLoop() {
    doMovement(camera, window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(1);

    renderer->render(window);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    fpsRenderer->drawText(std::to_string(window->calculateFPS()), glm::vec2(21.0f, 21.0f), 1.0f, glm::vec3(0.3f, 0.7f, 0.9f));

    glDisable(GL_BLEND);
    glutSwapBuffers();
}

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

