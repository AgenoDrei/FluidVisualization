SET(ALGORITHM_CPP Algorithms/BaseAlgorithm.cpp Algorithms/MarchingCubes.cpp Algorithms/TextureSlicing3D.cpp)
SET(ALGORITHM_H Algorithms/BaseAlgorithm.h Algorithms/MarchingCubes.h Algorithms/TextureSlicing3D.h)

SET(SRC_CPP main.cpp DataImporter.cpp DataSet.cpp Timestep.cpp Particle.cpp WindowHandler.cpp Shader.cpp Camera.cpp CpuInterpolationController.cpp RendererParticles.cpp RendererParticles.h RendererDebugQuad.cpp TextRenderer.cpp DataExporter.cpp OctreeInterpolationController.cpp OctreeNode.cpp MarchingCubesCalculator.cpp Grid.cpp GridCell.cpp Triangle.cpp RendererMarchingCubes.cpp MarchinCubesData.cpp VertexArrayBuffer.cpp MarchingCubesShader.cpp VertexPositionNormal.cpp VertexWelder.cpp MarchingCubesRenderObject.cpp Ground.cpp GroundShader.cpp MVPShader.cpp SkyBox.cpp SkyBoxShader.cpp TextureRenderer.cpp ReflectionCamera.cpp BaseCamera.cpp TextureSlicingRenderer.cpp FluidVisualisation.cpp TextureSlicer.cpp)
SET(SRC_H main.h DataImporter.h DataSet.h Timestep.h Particle.h WindowHandler.h Shader.h Camera.h InterpolationController.h CpuInterpolationController.h RendererParticles.cpp RendererParticles.h RendererDebugQuad.h TextRenderer.h  DataExporter.h  OctreeInterpolationController.h  OctreeNode.h MarchingCubesCalculator.h Grid.h GridCell.h Triangle.h RendererMarchingCubes.h MarchinCubesData.h VertexArrayBuffer.h MarchingCubesShader.h VertexPositionNormal.h VertexWelder.h MarchingCubesRenderObject.h Ground.h GroundShader.h MVPShader.h SkyBox.h SkyBoxShader.h TextureRenderer.h ReflectionCamera.h BaseCamera.h TextureSlicingRenderer.h FluidVisualisation.h TextureSlicer.h)

SET(SRC ${SRC_CPP} ${SRC_H} ${ALGORITHM_CPP} ${ALGORITHM_H})

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

#SOIL
find_package(SOIL REQUIRED)
include_directories(${SOIL_INCLUDE_DIRS})
target_link_libraries(FluidVisualisation ${SOIL_LIBRARY})

file(COPY ../shader DESTINATION ..)
file(COPY ../textures DESTINATION ..)

