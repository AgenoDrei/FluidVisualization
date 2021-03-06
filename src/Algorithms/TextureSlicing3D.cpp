#include <Configuration.h>
#include "TextureSlicing3D.h"
#include "TextureSlicer/TextureSlicer.h"
#include "Renderer/RendererTextureSlicing.h"
#include "WindowHandler.h"

TextureSlicing3D::TextureSlicing3D(BaseCamera* camera, uint dimX, uint dimY, uint dimZ, SkyBox* skyBox) {
    auto front = camera->getFront();

    std::unique_ptr<TextureSlicer> calculator(new TextureSlicer());
    _calculator = std::move(calculator);
    _calculator->sliceVolumedata(front);

    std::unique_ptr<TextureSlicingRenderer> renderer(new TextureSlicingRenderer(dimX, dimY, dimZ, skyBox));
    _renderer = std::move(renderer);
    _renderer->setBufferData(_calculator->getSlicedVolume());
    _renderer->viewDirOnSlicing = front;

    numSlices = 64;
    setNumSlices(numSlices);
}

TextureSlicing3D::~TextureSlicing3D() {}

void TextureSlicing3D::setNumSlices(int slices) {
    _calculator->setNumSlices(slices);
    _renderer->updateSizeofTextureSlicesVolume(slices);
    // now slice again with new num and old viewDir
    _calculator->sliceVolumedata(_renderer->viewDirOnSlicing);
    _renderer->setBufferData(_calculator->getSlicedVolume());
}

void TextureSlicing3D::init(Timestep* timestep) {
    _renderer->setTextureData(timestep);
}

void TextureSlicing3D::render(BaseCamera* camera, WindowHandler* windowHandler) {
    auto newFront = camera->getFront();
    if(newFront != _renderer->viewDirOnSlicing) {
        _calculator->sliceVolumedata(newFront);
        _renderer->setBufferData(_calculator->getSlicedVolume());
        _renderer->viewDirOnSlicing = newFront;
    }

    _renderer->render(camera, windowHandler);
}

std::string TextureSlicing3D::getName() const{
    return "Texture Slicing 3D";
}

void TextureSlicing3D::processKeyboard(WindowHandler* windowHandler) {
    if(windowHandler->getKeyDebounce('r')) {
        _renderer->toggleReflection();
        _reflectionEnabled = !_reflectionEnabled;
    }
    if(windowHandler->getKey('f')) {
        if (numSlices > 1) {
            numSlices -= 1;
            setNumSlices(numSlices);
        }
    }
    if(windowHandler->getKey('g')) {
        numSlices += 1;
        setNumSlices(numSlices);
    }
}

void TextureSlicing3D::setConfiguration(Configuration* configuration) {
    if(configuration->TextureSlicing3D.numSlices > 0)
        setNumSlices(configuration->TextureSlicing3D.numSlices);
    if (configuration->TextureSlicing3D.reflection) {
        _renderer->toggleReflection();
        _reflectionEnabled = !_reflectionEnabled;
    }
}

void TextureSlicing3D::nextTimestep(Timestep* step) {
    init(step);
};

void TextureSlicing3D::setLight(BaseLight* light) {
}

bool TextureSlicing3D::getShadows() const {
    return false;
}

bool TextureSlicing3D::getReflection() const {
    return _reflectionEnabled;
}
