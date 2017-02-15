#SET(SRC_CPP main.cpp datasetOperation/DataImporter.cpp DataSet.cpp Timestep.cpp Particle.cpp WindowHandler.cpp Shader.cpp Camera.cpp datasetOperation/CpuInterpolationController.cpp datasetOperation/GpuInterpolationController.cpp rendering/RendererParticles.cpp rendering/RendererParticles.h rendering/RendererDebugQuad.cpp rendering/TextRenderer.cpp datasetOperation/DataExporter.cpp)
#SET(SRC_H main.h datasetOperation/DataImporter.h DataSet.h Timestep.h Particle.h WindowHandler.h Shader.h Camera.h datasetOperation/InterpolationController.h datasetOperation/CpuInterpolationController.h datasetOperation/GpuInterpolationController.h rendering/RendererParticles.cpp rendering/RendererParticles.h rendering/RendererDebugQuad.h rendering/TextRenderer.h datasetOperation/DataExporter.h)
#SET(SRC ${SRC_CPP} ${SRC_H} rendering/Renderer3DTextureSlicing.cpp rendering/Renderer3DTextureSlicing.h )
SET(SRC main.cpp debug_stuff/GLSLShader.cpp debug_stuff/Grid.cpp debug_stuff/RenderableObject.cpp debug_stuff/GLSLShader.h debug_stuff/Grid.h debug_stuff/RenderableObject.h)

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

#file(COPY ../shader DESTINATION ..)
file(COPY debug_stuff/shaders DESTINATION ..)

