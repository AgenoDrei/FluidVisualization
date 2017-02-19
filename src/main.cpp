#include "main.h"
#include "DataImporter.h"
#include "DataExporter.h"
#include "DataSet.h"
#include "WindowHandler.h"
#include "Timestep.h"
#include "InterpolationController.h"
#include "CpuInterpolationController.h"
#include "RendererParticles.h"
#include "RendererDebugQuad.h"
#include "Renderer3DTextureSlicing.h"
#include "TextRenderer.h"
#include "OctreeInterpolationController.h"
#include "MarchingCubes.h"
#include "Grid.h"
#include "RendererMarchingCubes.h"
#include "VertexWelder.h"
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
DataSet* data = nullptr, *interpolatedData = nullptr;
InterpolationController *ctrl;
RendererParticles* renderer;
RendererDebugQuad* quadRenderer;
RendererMarchingCubes* marchingCubesRenderer;
Renderer3DTextureSlicing* textureSlicingRenderer;

int main(int argc, char* argv[]) {
    //std::string path = std::getenv("HOME"); //weird clion bug, not important for compiling
	//data = DataImporter::load("/home/nils/Downloads/drop_interpolation_300.dat");
    std::string path = std::getenv("HOME");
	data = DataImporter::load(path + "/Downloads/drop_100.dat");
    interpolatedData = data;

    //Window Initialisation
    window = new WindowHandler(800, 600);
    window->initWindow(argc, argv, &init, &mainLoop);

    //algorithm.draw();

    return 0;
}

void init() {
    std::cout << "Log> Render initialization running" << std::endl;

    camera = new Camera(glm::vec3(0.5f, 0.4f, 1.7f));
    window->setCamera(camera);

    //ctrl = new CpuInterpolationController(10);
    //ctrl = new OctreeInterpolationController(0.0025, 1.5);
    //interpolatedData = ctrl->interpolateData(data, 400, 100, 400);

    renderer = new RendererParticles();
    quadRenderer = new RendererDebugQuad();
    marchingCubesRenderer = new RendererMarchingCubes();
    textureSlicingRenderer = new Renderer3DTextureSlicing(1, 100, 100, 100, camera);

    //renderer->setData(interpolatedData->getTimestep(0), interpolatedData->getNumberParticles());
    //quadRenderer->setData(data->getTimestep(0), data->getNumberParticles());
    textureSlicingRenderer->setData(data->getTimestep(0));

//    auto firstTimestep = data->getTimestep(0);
//    auto grid = new Grid(firstTimestep->getParticleNumberPerDirection(), firstTimestep);
//
//    auto algorithm = MarchingCubes();
//    algorithm.calculate(grid);
//    auto vertices = algorithm.getVertices();
//
//    auto weldedResult = VertexWelder<VertexPositionNormal>::weld(vertices);
//    marchingCubesRenderer->addVertexIndexBuffer(weldedResult->VertexBuffer, weldedResult->IndexBuffer);

    fpsRenderer = new TextRenderer("../fonts/arial.ttf");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);       //    glCullFace(GL_BACK);
//    glEnable(GL_BLEND);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);       //    glPolygonMode(GL_FRONT, GL_LINE);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "Log> Initalization done" << std::endl;
}

void mainLoop() {
    doMovement(camera, window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(1);

    //renderer->render(camera, window);
    //quadRenderer->render(camera, window);     // somehow not working anymore; guess is shared rendering ..
    marchingCubesRenderer->render(camera, window);
    textureSlicingRenderer->render(window);

//    //using namespace std::chrono;        // slowing fps refresh down to ~ every .1 ms
//    //if (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() % 1000 > 900)
//    fps = window->calculateFPS();
//    //fpsRenderer->drawText(std::to_string(fps), glm::vec2(21.0f, 21.0f), 1.0f, glm::vec3(0.3f, 0.7f, 0.9f));
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

