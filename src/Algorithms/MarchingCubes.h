#pragma once

#include "BaseAlgorithm.h"

#include <memory>

class MarchingCubesCalculator;
class RendererMarchingCubes;
class SkyBox;

class MarchingCubes : public BaseAlgorithm {
protected:
    std::unique_ptr<MarchingCubesCalculator> _calculator;
    std::unique_ptr<RendererMarchingCubes> _renderer;
public:
    MarchingCubes(SkyBox* skyBox);

    ~MarchingCubes();

    virtual void init(Timestep* timestep) override;

    virtual void render(BaseCamera* camera, WindowHandler* windowHandler) override;

    virtual std::string getName() const override;

    virtual void processKeyboard(WindowHandler* windowHandler) override;
};



