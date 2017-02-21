#pragma once
#include <GL/gl.h>

class Camera;
class WindowHandler;
class DataSet;

//Abstract class (for Oli)
class InterpolationController {
public:
    virtual void prepareData(DataSet*) = 0; //maybe do texture stuff for gpu
    virtual DataSet* interpolateData(DataSet*, GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ) = 0;
private:
    virtual void compute(GLfloat resolutionX, GLfloat resolutionY, GLfloat resolutionZ) = 0;
};



