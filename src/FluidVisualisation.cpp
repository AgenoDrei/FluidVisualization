#include "FluidVisualisation.h"
#include "SkyBox.h"
#include "WindowHandler.h"
#include "Renderer/TextRenderer.h"

#include "Algorithms/MarchingCubes.h"
#include "Algorithms/TextureSlicing3D.h"

#include <algorithm>

FluidVisualisation::FluidVisualisation(Timestep* data) :
    _data(data) {
    _skyBox = new SkyBox();
    _textRenderer = new TextRenderer("../fonts/arial.ttf");

    _camera = new Camera(glm::vec3(0.5f, 0.4f, 1.7f));

    std::unique_ptr<BaseAlgorithm> marchingCubes(new MarchingCubes(_skyBox));
    _algorithms.push_back(std::move(marchingCubes));

    std::unique_ptr<BaseAlgorithm> textureSlicing3D(new TextureSlicing3D(_camera));
    _algorithms.push_back(std::move(textureSlicing3D));

    switchAlgorithm(&_algorithms.front());
}

FluidVisualisation::~FluidVisualisation() {
    delete _skyBox;
    delete _camera;
}

void FluidVisualisation::init(WindowHandler* windowHandler) {
    _windowHandler = windowHandler;
    _windowHandler->setCamera(_camera);
}

void FluidVisualisation::switchAlgorithm(std::unique_ptr<BaseAlgorithm>* newAlgorithm) {
    _currentAlgorithm = nullptr;

    newAlgorithm->get()->init(_data);

    _currentAlgorithm = newAlgorithm;
}

void FluidVisualisation::render() {
    _skyBox->render(_camera, _windowHandler);

    if(*_currentAlgorithm) { // only render the algorithm if one is selected
        _currentAlgorithm->get()->render(_camera, _windowHandler);
    }

    auto fps = _windowHandler->calculateFPS();
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
    if(_windowHandler->getKey('n')) {
        _camera->ProcessKeyboard(UP, deltaTime);
    }
    if(_windowHandler->getKey('m')) {
        _camera->ProcessKeyboard(DOWN, deltaTime);
    }

    if(_windowHandler->getKey('l')) {
        _nextKeyPresset = true;
    }
    if(!_windowHandler->getKey('l') && _nextKeyPresset) {
        _nextKeyPresset = false;
        auto place = std::find(_algorithms.begin(), _algorithms.end(), *_currentAlgorithm);
        place++;
        if(place == _algorithms.end()) {
            switchAlgorithm(&_algorithms.front());
        } else {
            switchAlgorithm(&(*place));
        }
    }
}
