#pragma once

#include <list>
#include <memory>

class WindowHandler;
class BaseAlgorithm;
class SkyBox;
class Camera;
class Timestep;
class TextRenderer;

class FluidVisualisation {
private:
    std::list<std::unique_ptr<BaseAlgorithm>> _algorithms;
    std::unique_ptr<BaseAlgorithm>* _currentAlgorithm;

    SkyBox* _skyBox;

    WindowHandler* _windowHandler;
    Camera* _camera;

    Timestep* _data;

    void switchAlgorithm(std::unique_ptr<BaseAlgorithm>* newAlgorithm);

    TextRenderer* _textRenderer;

    bool _nextKeyPresset;
public:
    FluidVisualisation(Timestep* data);

    ~FluidVisualisation();

    void init(WindowHandler* windowHandler);

    void render();

    void doMovement();
};



