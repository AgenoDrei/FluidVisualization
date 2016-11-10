#pragma once

class Timestep;

class MarchingCubes {
private:
    float _stepSize;
protected:
    void polygonise();
public:
    MarchingCubes(float stepSize);

    void calculate(Timestep* timestep);
};



