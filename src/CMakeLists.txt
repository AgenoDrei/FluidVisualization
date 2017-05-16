SET(ALGORITHM_MARCHING_CUBES_CPP Algorithms/MarchingCubes/Grid.cpp Algorithms/MarchingCubes/GridCell.cpp Algorithms/MarchingCubes/MarchinCubesData.cpp Algorithms/MarchingCubes/MarchingCubesCalculator.cpp Algorithms/MarchingCubes/MarchingCubesRenderObject.cpp Algorithms/MarchingCubes/Triangle.cpp Algorithms/MarchingCubes/VertexWelder.cpp Algorithms/RayCasting.cpp Algorithms/ParticlePoints.cpp)
SET(ALGORITHM_MARCHING_CUBES_H Algorithms/MarchingCubes/Grid.h Algorithms/MarchingCubes/GridCell.h Algorithms/MarchingCubes/MarchinCubesData.h Algorithms/MarchingCubes/MarchingCubesCalculator.h Algorithms/MarchingCubes/MarchingCubesRenderObject.h Algorithms/MarchingCubes/Triangle.h Algorithms/MarchingCubes/VertexWelder.h Algorithms/RayCasting.h Algorithms/ParticlePoints.h)

SET(ALGORITHM_TEXTURE_SLICER_CPP Algorithms/TextureSlicer/TextureSlicer.cpp)
SET(ALGORITHM_TEXTURE_SLICER_H Algorithms/TextureSlicer/TextureSlicer.h)

SET(ALGORITHM_CPP ${ALGORITHM_MARCHING_CUBES_CPP} ${ALGORITHM_TEXTURE_SLICER_CPP} Algorithms/BaseAlgorithm.cpp Algorithms/MarchingCubes.cpp Algorithms/TextureSlicing3D.cpp)
SET(ALGORITHM_H ${ALGORITHM_MARCHING_CUBES_H} ${ALGORITHM_TEXTURE_SLICER_H} Algorithms/BaseAlgorithm.h Algorithms/MarchingCubes.h Algorithms/TextureSlicing3D.h)

SET(CAMERAS_CPP Cameras/BaseCamera.cpp Cameras/Camera.cpp Cameras/ReflectionCamera.cpp)
SET(CAMERAS_H Cameras/BaseCamera.h Cameras/Camera.h Cameras/ReflectionCamera.h)

SET(DATA_MANAGEMENT_CPP DataManagement/CpuInterpolationController.cpp DataManagement/DataExporter.cpp DataManagement/DataImporter.cpp DataManagement/DataSet.cpp DataManagement/OctreeInterpolationController.cpp DataManagement/OctreeNode.cpp DataManagement/Particle.cpp DataManagement/Timestep.cpp)
SET(DATA_MANAGEMENT_H DataManagement/CpuInterpolationController.h DataManagement/DataExporter.h DataManagement/DataImporter.h DataManagement/DataSet.h DataManagement/OctreeInterpolationController.h DataManagement/OctreeNode.h DataManagement/Particle.h DataManagement/Timestep.h DataManagement/InterpolationController.h)

SET(RENDERER_CPP Renderer/RendererDebugQuad.cpp Renderer/RendererMarchingCubes.cpp Renderer/RendererParticles.cpp Renderer/TextureRenderer.cpp Renderer/TextRenderer.cpp Renderer/RendererTextureSlicing.cpp Renderer/RendererRayCasting.cpp)
SET(RENDERER_H Renderer/RendererDebugQuad.h Renderer/RendererMarchingCubes.h Renderer/RendererParticles.h Renderer/TextureRenderer.h Renderer/TextRenderer.h Renderer/RendererTextureSlicing.h Renderer/RendererRayCasting.h)

SET(LIGHTING_CPP Renderer/Lighting/BaseLight.cpp Renderer/Lighting/DirectionalLight.cpp)
SET(LIGHTING_H Renderer/Lighting/BaseLight.h Renderer/Lighting/DirectionalLight.h)

SET(SHADER_CPP Shader/ShadowMapShader.cpp Shader/GroundShader.cpp Shader/MarchingCubesShader.cpp Shader/EffectShader.cpp Shader/MVPShader.cpp Shader/Shader.cpp Shader/SkyBoxShader.cpp)
SET(SHADER_CH Shader/ShadowMapShader.h Shader/GroundShader.h Shader/MarchingCubesShader.h Shader/EffectShader.h Shader/MVPShader.h Shader/Shader.h Shader/SkyBoxShader.h)

SET(SRC_CPP main.cpp FluidVisualisation.cpp Ground.cpp SkyBox.cpp VertexPositionNormal.cpp VertexArrayBuffer.cpp WindowHandler.cpp Texture.cpp)
SET(SRC_H main.h FluidVisualisation.h Ground.h SkyBox.h VertexPositionNormal.h VertexArrayBuffer.h WindowHandler.h Texture.h)

SET(SRC ${SRC_CPP} ${SRC_H} ${ALGORITHM_CPP} ${ALGORITHM_H} ${CAMERAS_CPP} ${CAMERAS_H} ${DATA_MANAGEMENT_CPP} ${DATA_MANAGEMENT_H} ${RENDERER_CPP} ${RENDERER_H} ${SHADER_CPP} ${SHADER_H} ${LIGHTING_CPP} ${LIGHTING_H})

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

#Boost
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(FluidVisualisation ${Boost_LIBRARIES})

file(COPY ../shader DESTINATION ..)
file(COPY ../textures DESTINATION ..)

