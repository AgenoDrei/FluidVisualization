#include "MarchingCubes.h"
#include "Timestep.h"
#include "Grid.h"

MarchingCubes::MarchingCubes(float stepSize) :
    _stepSize(stepSize) {

}

void MarchingCubes::calculate(Timestep* timestep) {
    auto grid = Grid::CreateFromTimestep(timestep, _stepSize);

    auto test = 1;
}
