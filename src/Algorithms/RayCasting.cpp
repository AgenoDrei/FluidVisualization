//
// Created by simon on 24.02.17.
//

#include <Renderer/RendererRayCasting.h>
#include <memory>
#include <Configuration.h>
#include "RayCasting.h"
#include "DataManagement/Timestep.h"
#include "SkyBox.h"
#include "WindowHandler.h"

RayCasting::RayCasting(SkyBox* skyBox) {
    std::unique_ptr<RendererRayCasting> renderer(new RendererRayCasting(0.005f, skyBox));
    _renderer = std::move(renderer);
}

RayCasting::~RayCasting() {

}

void RayCasting::init(Timestep *timestep) {
    _renderer->setData(timestep, timestep->getSize());
}

void RayCasting::render(BaseCamera *camera, WindowHandler *windowHandler) {
    Camera* c = (Camera*) camera;
    _renderer->render(c, windowHandler);
}

std::string RayCasting::getName() const {
    return "Volume Ray Casting";
}

void RayCasting::processKeyboard(WindowHandler* windowHandler) {
    if(windowHandler->getKey('i')) {
        _renderer->changeStepSize(0.0001);
    }
    if(windowHandler->getKey('o')) {
        _renderer->changeStepSize(-0.0001);
    }
    if(windowHandler->getKeyDebounce('r')) {
        _renderer->toggleReflection();
    }
    if(windowHandler->getKeyDebounce('t')) {
        _renderer->toggleShadow();
    }
    if(windowHandler->getKeyDebounce('p')) {
        _renderer->changeLightPos();
    }

}

void RayCasting::setConfiguration(Configuration* configuration) {
//    conf = configuration;
    if(configuration->RayCasting.reflection)
        _renderer->toggleReflection();
    if(configuration->RayCasting.shadow)
        _renderer->toggleShadow();
}

void RayCasting::nextTimestep(Timestep* step) {
    init(step);
};
