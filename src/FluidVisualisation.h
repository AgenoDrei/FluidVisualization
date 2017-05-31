#pragma once

#include <vector>
#include <memory>
#include <Algorithms/BaseAlgorithm.h>

class WindowHandler;
class BaseAlgorithm;
class SkyBox;
class Camera;
class DataSet;
class Timestep;
class TextRenderer;
class Configuration;
class CSVWriter;

class FluidVisualisation {
private:
    std::vector<std::unique_ptr<BaseAlgorithm>> _algorithms;
    std::unique_ptr<BaseAlgorithm>* _currentAlgorithm;
    uint32_t _currentTimestep = 0;

    SkyBox* _skyBox;

    WindowHandler* _windowHandler;
    Camera* _camera;

    DataSet* _data;

    void switchAlgorithm(std::unique_ptr<BaseAlgorithm>* newAlgorithm);
    void nextTimestep();

    TextRenderer* _textRenderer;

    CSVWriter* _performanceWriter;
public:
    FluidVisualisation(DataSet* data, Configuration* configuration);

    ~FluidVisualisation();

    std::unique_ptr<BaseAlgorithm>* findAlgorithm(std::string name);

    void init(WindowHandler* windowHandler);

    void render();

    void doMovement();
};



