//
// Created by simon on 24.02.17.
//

#pragma once

#include "BaseAlgorithm.h"

class RendererRayCasting;

class RayCasting : public BaseAlgorithm {
protected:
    std::unique_ptr<RendererRayCasting> _renderer;
public:
    RayCasting();

    ~RayCasting();

    virtual void init(Timestep* timestep) override;

    virtual void render(BaseCamera* camera, WindowHandler* windowHandler) override;

    virtual std::string getName() const override;
};


