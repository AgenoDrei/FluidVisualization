#include "main.h"
#include "FluidVisualisation.h"
#include "WindowHandler.h"
#include "DataImporter.h"
#include "DataSet.h"

#include <iostream>

WindowHandler* window;
FluidVisualisation* fluidVisualisation;

int main(int argc, char* argv[]) {
    //Window Initialisation
    window = new WindowHandler(800, 600);
    window->initWindow(argc, argv, &init, &mainLoop);

    delete fluidVisualisation;
    delete window;

    return 0;
}

void init() {
    std::cout << "Log> Render initialization running" << std::endl;

    std::string path = std::getenv("HOME");
    auto data = DataImporter::load(path + "/Downloads/drop_100.dat");
    //auto interpolationController = new OctreeInterpolationController(0.0025, 1.5);
    //auto interpolatedData = interpolationController->interpolateData(data, 100, 100, 20);
    auto interpolatedData = data;
    auto firstTimestep = interpolatedData->getTimestep(0);
    //delete interpolationController; TODO: segfault --- simon whats going on? Create the controller on stack?

    fluidVisualisation = new FluidVisualisation(firstTimestep);

    fluidVisualisation->init(window);

    std::cout << "Log> Initalization done" << std::endl;
}

void mainLoop() {
    fluidVisualisation->doMovement();
    window->calculateFPS();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    fluidVisualisation->render();

    glDisable(GL_BLEND);
    glutSwapBuffers();
}


