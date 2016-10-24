SET(SRC_CPP main.cpp DataImporter.cpp DataSet.cpp Timestep.cpp Particle.cpp)
SET(SRC_H DataImporter.h DataSet.h Timestep.h Particle.h)

SET(SRC ${SRC_CPP} ${SRC_H})

#GLM Library
find_package(PkgConfig REQUIRED)
pkg_search_module(GLM REQUIRED glm)
include_directories(${GLM_INCLUDE_DIRS})

add_executable(FluidVisualisation ${SRC})

