#include "TextureSlicing3D.h"
#include "TextureSlicer/TextureSlicer.h"
#include "Renderer/TextureSlicingRenderer.h"

#include "WindowHandler.h"
#include "Cameras/BaseCamera.h"

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
    _initedDecSlicesPress = false;
    _initedIncSlicesPress = false;
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
    if(windowHandler->getKey('r')) {
        _renderer->enableReflection();
    }

    if(windowHandler->getKey('t')) {
        _renderer->disableReflection();
    }

    if(windowHandler->getKey('f')) {
        _initedDecSlicesPress = true;
    }
    if(windowHandler->getKey('g')) {
        _initedIncSlicesPress = true;
    }
    if(!windowHandler->getKey('f') && _initedDecSlicesPress) {
        if (numSlices > 1)
            numSlices /= 2;
        setNumSlices(numSlices);
        _initedDecSlicesPress = false;
    }
    if(!windowHandler->getKey('g') && _initedIncSlicesPress) {
        numSlices *= 2;
        setNumSlices(numSlices);
        _initedIncSlicesPress = false;
    }
}
