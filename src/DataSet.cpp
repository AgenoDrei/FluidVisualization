#include "DataSet.h"
#include "Timestep.h"

DataSet::DataSet(uint32_t numberParticles, uint32_t numberTimesteps) :
    _numberParticles(numberParticles),
    _numberTimesteps(numberTimesteps) {

    _timesteps = new Timestep*[numberTimesteps];
    for(auto i = 0u; i < numberTimesteps; i++) {
        _timesteps[i] = new Timestep(numberParticles);
    }
}

DataSet::DataSet(uint32_t numberParticles, uint32_t numberTimesteps, Timestep** timesteps) :
	_numberParticles(numberParticles),
	_numberTimesteps(numberTimesteps),
	_timesteps(timesteps) {
	
}

DataSet::~DataSet() {
    delete[] _timesteps;
}

uint32_t DataSet::getNumberParticles() const {
    return _numberParticles;
}

uint32_t DataSet::getNumberTimesteps() const {
    return _numberTimesteps;
}
