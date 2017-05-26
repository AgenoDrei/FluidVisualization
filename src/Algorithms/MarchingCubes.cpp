#include <Configuration.h>
#include "MarchingCubes.h"

#include "WindowHandler.h"

#include "Renderer/RendererMarchingCubes.h"

#include "DataManagement/Timestep.h"

#include "MarchingCubes/MarchingCubesCalculator.h"
#include "MarchingCubes/Grid.h"
#include "MarchingCubes/VertexWelder.h"
#include "Renderer/Lighting/DirectionalLight.h"

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
    if(windowHandler->getKeyDebounce('r')) {
        _renderer->toggleReflection();
        _reflectionEnabled = !_reflectionEnabled;
    }
    if(windowHandler->getKeyDebounce('t')) {
        _renderer->toggleShadow();
        _shadowsEnabled = !_shadowsEnabled;
    }
}

void MarchingCubes::setConfiguration(Configuration* configuration) {
    if(configuration->MarchingCubes.reflection) {
        _renderer->toggleReflection();
        _reflectionEnabled = !_reflectionEnabled;
    }
    if(configuration->MarchingCubes.shadow) {
        _renderer->toggleShadow();
        _shadowsEnabled = !_shadowsEnabled;
    }
}

void MarchingCubes::nextTimestep(Timestep* step) {
    init(step);
};

void MarchingCubes::setLight(BaseLight* light) {
    auto directionalLight = dynamic_cast<DirectionalLight*>(light);
    if(directionalLight) {  
        _renderer->setLight(directionalLight);
    }
}

bool MarchingCubes::getShadows() const {
    return _shadowsEnabled;
}

bool MarchingCubes::getReflection() const {
    return _reflectionEnabled;
}
