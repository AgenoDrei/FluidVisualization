#pragma once


#include <vector>
#include "Particle.h"
#include "DataSet.h"

class DataInterpolator {
public:
    static DataSet* interpolateDataset(DataSet data);
};