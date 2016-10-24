#pragma once

#include <string>
#include <fstream>

class DataSet;
class Timestep;
class Particle;

class DataImporter {
private:
    static Timestep* loadTimestep(std::ifstream& file, int32_t numberParticles);
    static Particle loadParticle(std::ifstream& file);
public:
    static DataSet* load(std::string path);
};
