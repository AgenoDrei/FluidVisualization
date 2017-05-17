#pragma once

#include "BaseAlgorithm.h"

class RendererParticles;

class ParticlePoints : public BaseAlgorithm {
protected:
    std::unique_ptr<RendererParticles> _renderer;
public:
    ParticlePoints();

    ~ParticlePoints();

    virtual void init(Timestep* timestep) override;

    virtual void render(BaseCamera* camera, WindowHandler* windowHandler) override;

    virtual std::string getName() const override;

    virtual void processKeyboard(WindowHandler* windowHandler) override;

    virtual void setConfiguration(Configuration* configuration) override;

    virtual void nextTimestep(Timestep* step) override;

    virtual void setLight(BaseLight* light) override;
};