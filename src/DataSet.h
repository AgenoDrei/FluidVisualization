#pragma once

#include <cstdint>

class Timestep;

class DataSet {
private:
    uint32_t _numberParticles;
    uint32_t _numberTimesteps;

    Timestep** _timesteps;

public:
    DataSet(uint32_t numberParticles, uint32_t numberTimesteps);
	DataSet(uint32_t numberParticles, uint32_t numberTimesteps, Timestep** timesteps);
    ~DataSet();

    uint32_t getNumberParticles() const;
    uint32_t getNumberTimesteps() const;
    Timestep* getTimestep(uint32_t index) const;
};
