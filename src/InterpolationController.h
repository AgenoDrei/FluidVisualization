#pragma once
#include <GL/gl.h>

class Camera;
class WindowHandler;
class DataSet;

//Abstract class (for Oli)
class InterpolationController {
public:
    virtual void createShader() = 0;
    virtual void prepareGpuBuffer(DataSet*, uint32_t) = 0; //maybe do texture stuff for gpu
    virtual void loadGpuBuffer() = 0;
    virtual void renderParticles(Camera*, WindowHandler*) = 0;
};



