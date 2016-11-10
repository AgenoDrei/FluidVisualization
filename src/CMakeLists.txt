SET(SRC_CPP main.cpp DataImporter.cpp DataSet.cpp Timestep.cpp Particle.cpp WindowHandler.cpp Shader.cpp Camera.cpp MarchingCubes.cpp Grid.cpp GridCell.cpp)
SET(SRC_H main.h DataImporter.h DataSet.h Timestep.h Particle.h WindowHandler.h Shader.h Camera.h MarchingCubes.h Grid.h GridCell.h)

SET(SRC ${SRC_CPP} ${SRC_H})

add_executable(FluidVisualisation ${SRC})

#GLM Library
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

#Freeglut Library
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
add_definitions(${GLUT_DEFINITIONS})
target_link_libraries(FluidVisualisation ${GLUT_LIBRARY})

#GLEW Library
find_package(GLEW REQUIRED)
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(FluidVisualisation ${GLEW_LIBRARIES})

#opengl
find_package(OpenGL REQUIRED)
target_link_libraries(FluidVisualisation ${OPENGL_gl_LIBRARY})

file(COPY ../shader DESTINATION .)

