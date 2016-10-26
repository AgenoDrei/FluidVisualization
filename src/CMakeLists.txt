SET(SRC_CPP main.cpp DataImporter.cpp DataSet.cpp Timestep.cpp Particle.cpp WindowHandler.cpp Shader.cpp Camera.cpp)
SET(SRC_H main.h DataImporter.h DataSet.h Timestep.h Particle.h WindowHandler.h Shader.h Camera.h)

SET(SRC ${SRC_CPP} ${SRC_H})

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

file(COPY ../shader DESTINATION .)

