#pragma once

#include <string>

class BaseCamera;
class WindowHandler;
class Timestep;

class BaseAlgorithm {
protected:
    BaseAlgorithm();

public:
    virtual void init(Timestep* timestep) = 0;

    virtual void render(BaseCamera* camera, WindowHandler* windowHandler) = 0;

    virtual std::string getName() const = 0;
};



