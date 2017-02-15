#pragma once
#include <GL/gl.h>

class Camera;
class WindowHandler;
class DataSet;

//Abstract class (for Oli)
class InterpolationController {
public:
    virtual void prepareData(DataSet*) = 0; //maybe do texture stuff for gpu
    virtual DataSet* interpolateData(DataSet*) = 0;
private:
    virtual void compute() = 0;
};



