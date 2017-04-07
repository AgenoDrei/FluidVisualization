#pragma once

#include <string>

class Configuration {
public:
    std::string pathToData;
    std::string algorithm;

    struct {
        bool reflection;
    } MarchingCubes;

    struct {
        int numSlices;
    } TextureSlicing3D;
};
