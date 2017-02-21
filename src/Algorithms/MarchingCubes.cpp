#include "MarchingCubes.h"
#include "MarchingCubesCalculator.h"
#include "RendererMarchingCubes.h"
#include "Timestep.h"
#include "Grid.h"
#include "VertexWelder.h"

MarchingCubes::MarchingCubes(SkyBox* skyBox) {
    std::unique_ptr<MarchingCubesCalculator> calculator(new MarchingCubesCalculator());
    _calculator = std::move(calculator);

    std::unique_ptr<RendererMarchingCubes> renderer(new RendererMarchingCubes(skyBox));
    _renderer = std::move(renderer);
}

MarchingCubes::~MarchingCubes() {
}

void MarchingCubes::init(Timestep* timestep) {
    _renderer->clean();

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
