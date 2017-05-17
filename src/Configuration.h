#pragma once

#include <string>

class Configuration {
public:
    std::string pathToData;
    std::string algorithm;
    bool interpolation;

    struct {
        bool reflection;
        bool shadow;
    } MarchingCubes;

    struct {
        bool reflection;
        int numSlices;
    } TextureSlicing3D;

    struct {
        bool reflection;
        bool shadow;
    } RayCasting;
};
