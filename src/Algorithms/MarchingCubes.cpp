#include "MarchingCubes.h"
#include "MarchingCubesCalculator.h"
#include "RendererMarchingCubes.h"
#include "Timestep.h"
#include "Grid.h"
#include "VertexWelder.h"

MarchingCubes::MarchingCubes(SkyBox* skyBox) {
    _calculator = new MarchingCubesCalculator();
    _renderer = new RendererMarchingCubes(skyBox);
}

MarchingCubes::~MarchingCubes() {
    delete _calculator;
    delete _renderer;
}

void MarchingCubes::init(Timestep* timestep) {
    Grid grid(timestep->getParticleNumberPerDirection(), timestep);

    _calculator->calculate(&grid);
    auto vertices = _calculator->getVertices();

    auto weldedResult = VertexWelder<VertexPositionNormal>::weld(vertices);
    _renderer->addVertexIndexBuffer(weldedResult->VertexBuffer, weldedResult->IndexBuffer);
}

void MarchingCubes::render(BaseCamera* camera, WindowHandler* windowHandler)  {
    _renderer->render(camera, windowHandler);
}

std::string MarchingCubes::getName() const {
    return "Marching Cubes";
};
