//
// Created by simon on 24.02.17.
//

#include <Renderer/RendererParticles.h>
#include <memory>
#include "ParticlePoints.h"
#include "DataManagement/Timestep.h"

ParticlePoints::ParticlePoints() {
    std::unique_ptr<RendererParticles> renderer(new RendererParticles());
    _renderer = std::move(renderer);
}

ParticlePoints::~ParticlePoints() {

}

void ParticlePoints::init(Timestep *timestep) {
    _renderer->setData(timestep, timestep->getSize());
}

void ParticlePoints::render(BaseCamera *camera, WindowHandler *windowHandler) {
    Camera* c = (Camera*) camera;
    _renderer->render(c, windowHandler);
}

std::string ParticlePoints::getName() const {
    return "Particle Points";
}

void ParticlePoints::processKeyboard(WindowHandler* windowHandler) {

}

void ParticlePoints::setConfiguration(Configuration* configuration) {

}

void ParticlePoints::nextTimestep(Timestep* step) {

};
