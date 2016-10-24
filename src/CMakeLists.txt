SET(SRC_CPP main.cpp DataImporter.cpp DataSet.cpp Timestep.cpp Particle.cpp)
SET(SRC_H DataImporter.h DataSet.h Timestep.h Particle.h)

SET(SRC ${SRC_CPP} ${SRC_H} WindowHandler.cpp WindowHandler.h WindowHandler.cpp WindowHandler.h Shader.cpp Shader.h Camera.cpp Camera.h)

add_executable(FluidVisualisation ${SRC})

#GLM Library
find_package(PkgConfig REQUIRED)
pkg_search_module(GLM REQUIRED glm)
include_directories(${GLM_INCLUDE_DIRS})

#Freeglut Library
find_package(PkgConfig REQUIRED)
pkg_search_module(GLUT REQUIRED freeglut)
include_directories(${GLUT_INCLUDE_DIRS})
target_link_libraries(FluidVisualisation ${GLUT_STATIC_LIBRARIES})

#GLEW Library
find_package(GLEW REQUIRED)
if(GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIRS})
  target_link_libraries(FluidVisualisation ${GLEW_LIBRARIES})
endif()

