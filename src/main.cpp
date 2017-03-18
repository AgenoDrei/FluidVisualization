#include "main.h"
#include "FluidVisualisation.h"
#include "WindowHandler.h"
#include "DataManagement/DataImporter.h"
#include "DataManagement/DataSet.h"
#include <iostream>
#include <string>
#include <DataManagement/CpuInterpolationController.h>
#include <DataManagement/DataExporter.h>

WindowHandler* window;
FluidVisualisation* fluidVisualisation;

class InitParameter {
public:
    std::string pathToData;
    std::string algorithm;
};

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    po::options_description desc("Allowed options");
    auto vm = setupCommandLine(argc, argv, desc);
    if(vm.count("help")) {
        printHelp(desc);
        return 0;
    }

    if(vm.count("input-file") < 1) {
        std::cout<<"No input file specified"<<std::endl;
        return -1;
    }

    InitParameter parameter;
    parameter.pathToData = vm["input-file"].as<std::string>();
    if(vm.count("algorithm")) {
        parameter.algorithm = vm["algorithm"].as<std::string>();
    } else {
        parameter.algorithm = "pointCloud";
    }

    std::cout<<"Input file: "<<parameter.pathToData<<std::endl;

    //Window Initialisation
    window = new WindowHandler(1024, 768);
    window->initWindow(argc, argv, &init, &mainLoop, &parameter);

    delete fluidVisualisation;
    delete window;

    return 0;
}

void printHelp(boost::program_options::options_description& desc) {
    std::cout<<desc<<std::endl;
}

po::variables_map setupCommandLine(int argc, char* argv[], po::options_description& desc) {
    desc.add_options()
            ("help", "produce help message")
            ("input-file", po::value<std::string>(), "input file")
            ("algorithm", po::value<std::string>(), "Default Algorithm")
            ;

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    return vm;
}

void init(InitParameter* parameter) {
    std::cout << "Log> FluidVisualization init running" << std::endl;

    /*std::string path = std::getenv("HOME");
    auto data = DataImporter::load(path + "/Downloads/drop_100.dat");*/
    auto data = DataImporter::load(parameter->pathToData);
    //auto interpolationController = new OctreeInterpolationController(0.0025, 1.5);
    //auto interpolatedData = interpolationController->interpolateData(data, 100, 20, 100);
    auto interpolatedData = data;
    auto firstTimestep = interpolatedData->getTimestep(0);
    //DataExporter::write(path + "/Downloads/drop_normals_100.dat", interpolatedData);
    //delete interpolationController; TODO: segfault --- simon whats going on? Create the controller on stack?

    fluidVisualisation = new FluidVisualisation(firstTimestep, parameter->algorithm);
    fluidVisualisation->init(window);

    std::cout << "Log> FluidVisualization init done" << std::endl;
}

void mainLoop() {
    fluidVisualisation->doMovement();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fluidVisualisation->render();
    glutSwapBuffers();
}


