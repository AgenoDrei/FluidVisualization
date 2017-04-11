#pragma once

#include <string>

class BaseCamera;
class WindowHandler;
class Timestep;
class Configuration;

class BaseAlgorithm {
protected:
    BaseAlgorithm();

public:
    virtual void init(Timestep* timestep) = 0;

    virtual void render(BaseCamera* camera, WindowHandler* windowHandler) = 0;

    virtual std::string getName() const = 0;

    virtual void processKeyboard(WindowHandler* windowHandler) = 0;

    virtual void setConfiguration(Configuration* configuration) = 0;

    virtual void nextTimestep(Timestep* step) = 0;
};



