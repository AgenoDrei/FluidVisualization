SET(SRC_CPP main.cpp DataImporter.cpp DataSet.cpp Timestep.cpp Particle.cpp WindowHandler.cpp Shader.cpp Camera.cpp CpuInterpolationController.cpp GpuInterpolationController.cpp RendererParticles.cpp RendererParticles.h RendererDebugQuad.cpp MarchingCubes.cpp Grid.cpp GridCell.cpp Triangle.cpp)
SET(SRC_H main.h DataImporter.h DataSet.h Timestep.h Particle.h WindowHandler.h Shader.h Camera.h InterpolationController.h CpuInterpolationController.h GpuInterpolationController.h RendererParticles.cpp RendererParticles.h RendererDebugQuad.h MarchingCubes.h Grid.h GridCell.h Triangle.h)

SET(SRC ${SRC_CPP} ${SRC_H} TextRenderer.cpp TextRenderer.h)

add_executable(FluidVisualisation ${SRC})

#Freetype
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

#GLM Library
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
target_link_libraries(FluidVisualisation ${FREETYPE_LIBRARIES})

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

file(COPY ../shader DESTINATION ..)

