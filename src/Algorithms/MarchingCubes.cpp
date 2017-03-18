#include "MarchingCubes.h"

#include "WindowHandler.h"

#include "Renderer/RendererMarchingCubes.h"

#include "DataManagement/Timestep.h"

#include "MarchingCubes/MarchingCubesCalculator.h"
#include "MarchingCubes/Grid.h"
#include "MarchingCubes/VertexWelder.h"

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

void MarchingCubes::processKeyboard(WindowHandler* windowHandler) {
    if(windowHandler->getKey('r')) {
        _renderer->enableReflection();
    }

    if(windowHandler->getKey('t')) {
        _renderer->disableReflection();
    }
}
