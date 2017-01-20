#pragma once

#include <string>
#include <fstream>
#include "Particle.h"

class DataSet;
class Timestep;

class DataExporter {
private:
    static void writeTimestep(std::ofstream& file, int32_t numberParticles, Timestep* timestep);
    static void writeParticle(std::ofstream& file, Particle particle);
public:
    static bool write(std::string path, DataSet* data);
};


