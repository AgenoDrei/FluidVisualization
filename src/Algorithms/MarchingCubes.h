#pragma once

#include "BaseAlgorithm.h"

class MarchingCubesCalculator;
class RendererMarchingCubes;
class SkyBox;

class MarchingCubes : public BaseAlgorithm {
protected:
    MarchingCubesCalculator* _calculator;
    RendererMarchingCubes* _renderer;
public:
    MarchingCubes(SkyBox* skyBox);

    ~MarchingCubes();

    virtual void init(Timestep* timestep) override;

    virtual void render(BaseCamera* camera, WindowHandler* windowHandler) override;

    virtual std::string getName() const override;
};



