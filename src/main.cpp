#include "main.h"
#include "FluidVisualisation.h"
#include "WindowHandler.h"
#include "DataManagement/DataImporter.h"
#include "DataManagement/DataSet.h"
#include <iostream>
#include <DataManagement/CpuInterpolationController.h>
#include <DataManagement/DataExporter.h>
#include <DataManagement/OctreeInterpolationController.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "Configuration.h"

WindowHandler* window;
FluidVisualisation* fluidVisualisation;


namespace po = boost::program_options;

int main(int argc, const char* argv[]) {
    Configuration parameter;

    std::ifstream file("config.ini", std::ifstream::in);
    if(!file.fail()) {
        file.close();

        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini("config.ini", pt);

        if(pt.get_child_optional("Main.StartAlgorithm")) {
            parameter.algorithm = pt.get<std::string>("Main.StartAlgorithm");
        }
        if(pt.get_child_optional("Main.InputFile")) {
            parameter.pathToData = pt.get<std::string>("Main.InputFile");
        }
        if(pt.get_child_optional("Main.Interpolation")) {
            parameter.interpolation = pt.get<bool>("Main.Interpolation");
        }

        if(pt.get_child_optional("MarchingCube.Reflection")) {
            parameter.MarchingCubes.reflection = pt.get<bool>("MarchingCube.Reflection");
        }
        if(pt.get_child_optional("MarchingCube.Shadow")) {
            parameter.MarchingCubes.shadow = pt.get<bool>("MarchingCube.Shadow");
        }

        if(pt.get_child_optional("TextureSlicing3D.Reflection")) {
            parameter.TextureSlicing3D.reflection = pt.get<bool>("TextureSlicing3D.Reflection");
        }
        if(pt.get_child_optional("TextureSlicing3D.NumSlices")) {
            parameter.TextureSlicing3D.numSlices = pt.get<int>("TextureSlicing3D.NumSlices");
        } else {
            parameter.TextureSlicing3D.numSlices = -1;
        }

        if(pt.get_child_optional("RayCasting.Reflection")) {
            parameter.RayCasting.reflection = pt.get<bool>("RayCasting.Reflection");
        }
        if(pt.get_child_optional("RayCasting.Shadow")) {
            parameter.RayCasting.shadow = pt.get<bool>("RayCasting.Shadow");
        }

    } else {
        parameter.MarchingCubes.reflection = false;
        parameter.MarchingCubes.shadow = false;
        parameter.TextureSlicing3D.reflection = false;
        parameter.TextureSlicing3D.numSlices = 42;
        parameter.RayCasting.reflection = false;
        parameter.RayCasting.shadow = false;
    }

    po::options_description desc("Allowed options");
    auto vm = setupCommandLine(argc, argv, desc);
    if(vm.count("help")) {
        printHelp(desc);
        return 0;
    }

    if(vm.count("input-file") < 1 && parameter.pathToData == "") {
        std::cout<<"No input file specified"<<std::endl;
        return -1;
    }

    if(vm.count("input-file") >= 1) {
        parameter.pathToData = vm["input-file"].as<std::string>();
    }
    if(vm.count("algorithm")) {
        std::cout<<"New algorithm"<<std::endl;
        parameter.algorithm = vm["algorithm"].as<std::string>();
    } else if(parameter.algorithm == "") {
        parameter.algorithm = "pointCloud";
    }
    if(vm.count("interpolation")) {
        parameter.interpolation = vm["interpolation"].as<std::string>() == "true" ? true : false;
    }

    std::cout<<"Input file: "<<parameter.pathToData<<std::endl;

    //Window Initialisation
    window = new WindowHandler(800, 600);
    window->initWindow(argc, argv, &init, &mainLoop, &parameter);

    delete fluidVisualisation;
    delete window;

    return 0;
}

void printHelp(boost::program_options::options_description& desc) {
    std::cout<<desc<<std::endl;
}

po::variables_map setupCommandLine(int argc, const char* argv[], po::options_description& desc) {
    desc.add_options()
            ("help", "produce help message")
            ("input-file", po::value<std::string>(), "input file")
            ("algorithm", po::value<std::string>(), "Default Algorithm")
            ("interpolation", po::value<std::string>(), "Interpolation")
            ;

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    return vm;
}

void init(Configuration* parameter) {
    std::cout << "Log> FluidVisualization init running" << std::endl;

    auto data = DataImporter::load(parameter->pathToData);
    auto interpolationController = new OctreeInterpolationController(0.01, 1.0);
    auto interpolatedData = data;
    if(parameter->interpolation)
        interpolatedData = interpolationController->interpolateData(data, 200, 200, 200);

    fluidVisualisation = new FluidVisualisation(interpolatedData, parameter);
    fluidVisualisation->init(window);

    std::cout << "Log> FluidVisualization init done" << std::endl;
}

void mainLoop() {
    fluidVisualisation->doMovement();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fluidVisualisation->render();
    glutSwapBuffers();
}


