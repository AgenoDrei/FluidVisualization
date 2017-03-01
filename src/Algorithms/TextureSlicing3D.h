#pragma once

#include "BaseAlgorithm.h"

#include <memory>

class TextureSlicer;
class TextureSlicingRenderer;
class BaseCamera;

class TextureSlicing3D : public BaseAlgorithm {
protected:
    std::unique_ptr<TextureSlicer> _calculator;
    std::unique_ptr<TextureSlicingRenderer> _renderer;
public:
    TextureSlicing3D(BaseCamera* camera, uint dimX, uint dimY, uint dimZ);

    ~TextureSlicing3D();

    virtual void init(Timestep* timestep) override;

    virtual void render(BaseCamera* camera, WindowHandler* windowHandler) override;

    virtual std::string getName() const override;

    void setNumSlices(int slices);
};



