#pragma once
#include <GL/gl.h>

class Camera;
class WindowHandler;
class DataSet;
class Timestep;

//Abstract class (for Oli)
class InterpolationController {
public:
    virtual DataSet* interpolateData(DataSet*, GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ) = 0;
private:
    virtual void prepareData(Timestep*) = 0;
    virtual void compute(GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ) = 0;
};



