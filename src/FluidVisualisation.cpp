#include "FluidVisualisation.h"
#include "SkyBox.h"
#include "WindowHandler.h"
#include "Renderer/TextRenderer.h"
#include "DataManagement/Timestep.h"
#include "DataManagement/DataSet.h"

#include "Algorithms/MarchingCubes.h"
#include "Algorithms/TextureSlicing3D.h"
#include "Algorithms/RayCasting.h"
#include "Configuration.h"
#include "CSVWriter.h"
#include <chrono>

#include <algorithm>
#include <Algorithms/ParticlePoints.h>

#include <iostream>
#include <Renderer/Lighting/DirectionalLight.h>

FluidVisualisation::FluidVisualisation(DataSet* data, Configuration* configuration) :
    _data(data) {
    glm::vec3 partNumsPerDir = _data->getTimestep(_currentTimestep)->getParticleNumberPerDirection();
    _skyBox = new SkyBox();
    _textRenderer = new TextRenderer("../fonts/arial.ttf");

    _camera = new Camera(glm::vec3(0.5f, 0.4f, 1.7f));

    auto light = new DirectionalLight();

    std::unique_ptr<BaseAlgorithm> particlePoints(new ParticlePoints());
    particlePoints->setConfiguration(configuration);
    std::unique_ptr<BaseAlgorithm> marchingCubes(new MarchingCubes(_skyBox));
    marchingCubes->setLight(light);
    marchingCubes->setConfiguration(configuration);
    std::unique_ptr<BaseAlgorithm> textureSlicing3D(new TextureSlicing3D(_camera, (uint)partNumsPerDir.x, (uint)partNumsPerDir.y, (uint)partNumsPerDir.z, _skyBox));
    textureSlicing3D->setConfiguration(configuration);
    std::unique_ptr<BaseAlgorithm> rayCasting(new RayCasting(_skyBox));
    rayCasting->setConfiguration(configuration);

    _algorithms.push_back(std::move(particlePoints));
    _algorithms.push_back(std::move(marchingCubes));
    _algorithms.push_back(std::move(rayCasting));
    _algorithms.push_back(std::move(textureSlicing3D));

    switchAlgorithm(findAlgorithm(configuration->algorithm));

    _performanceWriter = new CSVWriter("performance.csv");
}

FluidVisualisation::~FluidVisualisation() {
    delete _skyBox;
    delete _camera;
    delete _performanceWriter;
}

std::unique_ptr<BaseAlgorithm>* FluidVisualisation::findAlgorithm(std::string name) {
    for(auto it = _algorithms.begin(); it != _algorithms.end(); it++) {
        if(it->get()->getName() == name) {
            return &(*it);
        }
    }
    return &_algorithms.front();
}

void FluidVisualisation::init(WindowHandler* windowHandler) {
    _windowHandler = windowHandler;
    _windowHandler->setCamera(_camera);
}

void FluidVisualisation::switchAlgorithm(std::unique_ptr<BaseAlgorithm>* newAlgorithm) {
    _currentAlgorithm = nullptr;

    newAlgorithm->get()->init(_data->getTimestep(_currentTimestep));

    _currentAlgorithm = newAlgorithm;
}

void FluidVisualisation::nextTimestep() {
    _currentTimestep = (_currentTimestep + 1) % _data->getNumberTimesteps();
    std::cout << "Current: " << _currentTimestep << " | " << _data->getNumberTimesteps() << std::endl;
    _currentAlgorithm->get()->nextTimestep(_data->getTimestep(_currentTimestep));
}

void FluidVisualisation::render() {
    _skyBox->render(_camera, _windowHandler);

    std::chrono::high_resolution_clock::time_point start, finish;
    if(*_currentAlgorithm) { // only render the algorithm if one is selected
        start = std::chrono::high_resolution_clock::now();
        _currentAlgorithm->get()->render(_camera, _windowHandler);
        finish = std::chrono::high_resolution_clock::now();
    }

    auto fps = _windowHandler->calculateFPS();

    if(*_currentAlgorithm) {
        _performanceWriter->writeFrame(_currentAlgorithm->get()->getName(), std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count(), fps, _currentAlgorithm->get()->getReflection(), _currentAlgorithm->get()->getShadows());
    }

    _textRenderer->drawText(std::to_string(fps), glm::vec2(21.0f, 21.0f), 1.0f, glm::vec3(0.3f, 0.7f, 0.9f));

    _textRenderer->drawText("Current Alogirthm: " + _currentAlgorithm->get()->getName(), glm::vec2(21.0f, 550.0f), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void FluidVisualisation::doMovement() {
    auto deltaTime = static_cast<float>(_windowHandler->getDeltaTime());
    if(_windowHandler->getKey('w')) {
        _camera->ProcessKeyboard(FORWARD, deltaTime);
    }
    if(_windowHandler->getKey('s')) {
        _camera->ProcessKeyboard(BACKWARD, deltaTime);
    }
    if(_windowHandler->getKey('a')) {
        _camera->ProcessKeyboard(LEFT, deltaTime);
    }
    if(_windowHandler->getKey('d')) {
        _camera->ProcessKeyboard(RIGHT, deltaTime);
    }
    if(_windowHandler->getKey('n') || _windowHandler->getSpecialKey(GLUT_KEY_PAGE_UP)) {
        _camera->ProcessKeyboard(UP, deltaTime);
    }
    if(_windowHandler->getKey('m') || _windowHandler->getSpecialKey(GLUT_KEY_PAGE_DOWN)) {
        _camera->ProcessKeyboard(DOWN, deltaTime);
    }

    if(_windowHandler->getKeyDebounce('l')) {
        std::cout<<"Switching algorithm"<<std::endl;
        auto place = std::find(_algorithms.begin(), _algorithms.end(), *_currentAlgorithm);
        place++;
        if(place == _algorithms.end()) {
            switchAlgorithm(&_algorithms.front());
        } else {
            switchAlgorithm(&(*place));
        }
    }
    if(_windowHandler->getKeyDebounce('k')) {
        std::cout<<"Switching Timestep"<<std::endl;
        nextTimestep();
    }

    _currentAlgorithm->get()->processKeyboard(_windowHandler);
}
