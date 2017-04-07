//
// Created by simon on 24.02.17.
//

#include <Renderer/RendererRayCasting.h>
#include <memory>
#include "RayCasting.h"
#include "DataManagement/Timestep.h"
#include "SkyBox.h"

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

}

void RayCasting::setConfiguration(Configuration* configuration) {

}
