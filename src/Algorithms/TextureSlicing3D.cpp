#include "TextureSlicing3D.h"

#include "TextureSlicer/TextureSlicer.h"

#include "Renderer/TextureSlicingRenderer.h"

#include "Cameras/BaseCamera.h"

TextureSlicing3D::TextureSlicing3D(BaseCamera* camera, uint dimX, uint dimY, uint dimZ) {
    auto front = camera->getFront();

    std::unique_ptr<TextureSlicer> calculator(new TextureSlicer());
    _calculator = std::move(calculator);
    _calculator->sliceVolumedata(front);

    std::unique_ptr<TextureSlicingRenderer> renderer(new TextureSlicingRenderer(dimX, dimY, dimZ));
    _renderer = std::move(renderer);
    _renderer->setBufferData(_calculator->getSlicedVolume());
    _renderer->viewDirOnSlicing = front;

    setNumSlices(128);
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
