//
// Created by simon on 24.02.17.
//

#pragma once

#include "BaseAlgorithm.h"

class RendererRayCasting;
class SkyBox;
class Configuration;

class RayCasting : public BaseAlgorithm {
private:
    bool _reflectionEnabled = true;
    bool _shadowsEnabled = true;
protected:
    std::unique_ptr<RendererRayCasting> _renderer;
    Configuration* conf;
public:
    RayCasting(SkyBox*);

    ~RayCasting();

    virtual void init(Timestep* timestep) override;

    virtual void render(BaseCamera* camera, WindowHandler* windowHandler) override;

    virtual std::string getName() const override;
    virtual bool getReflection() const override;
    virtual bool getShadows() const override;

    virtual void processKeyboard(WindowHandler* windowHandler) override;

    virtual void setConfiguration(Configuration* configuration) override;

    virtual void nextTimestep(Timestep* step) override;

    virtual void setLight(BaseLight* light) override;
};


