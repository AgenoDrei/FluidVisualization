#pragma once

#include <vector>
#include <memory>
#include <Algorithms/BaseAlgorithm.h>

class WindowHandler;
class BaseAlgorithm;
class SkyBox;
class Camera;
class Timestep;
class TextRenderer;

class FluidVisualisation {
private:
    std::vector<std::unique_ptr<BaseAlgorithm>> _algorithms;
    std::unique_ptr<BaseAlgorithm>* _currentAlgorithm;

    SkyBox* _skyBox;

    WindowHandler* _windowHandler;
    Camera* _camera;

    Timestep* _data;

    void switchAlgorithm(std::unique_ptr<BaseAlgorithm>* newAlgorithm);

    TextRenderer* _textRenderer;

    bool _nextKeyPresset;
public:
    FluidVisualisation(Timestep* data, std::string startAlgorithm);

    ~FluidVisualisation();

    std::unique_ptr<BaseAlgorithm>* findAlgorithm(std::string name);

    void init(WindowHandler* windowHandler);

    void render();

    void doMovement();
};



