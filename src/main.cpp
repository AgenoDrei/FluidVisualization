#include "main.h"
#include "DataImporter.h"
#include "DataSet.h"
#include "WindowHandler.h"
#include "Camera.h"
#include "Shader.h"
#include "Timestep.h"
#include "ParticleGpuLoader.h"
#include "DataInterpolator.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>
#include <iostream>


WindowHandler* window;
Camera* camera;
GLuint VBO, VAO;
Shader* ourShader;
DataSet* data = nullptr, *data2 = nullptr;

GLuint texture;

int main(int argc, char* argv[]) {
	//data = DataImporter::load("/home/simon/Downloads/drop.dat");
	data = DataImporter::load("/home/nils/Downloads/drop.dat");

    //data2 = DataInterpolator::interpolateDataset(*data);
    data2 = data;

    //Window Initialisation
    window = new WindowHandler(800, 600);
    window->initWindow(argc, argv, &init, &mainLoop);
    return 0;
}

void init() {
    std::cout << "Log> Render initialization running" << std::endl;
    camera = new Camera(glm::vec3(0.5f, 0.4f, 1.7f));
    window->setCamera(camera);
    //ourShader = new Shader("shader/basic.vert", "shader/basic.frag");

    ourShader =  new Shader("shader/interpolater.vert", "shader/interpolater.frag");

    auto gridSize = 10;
    auto arraySize = gridSize * gridSize * gridSize;
    auto cubeVertices = new glm::vec3[arraySize];
    auto i = 0;
    for(float z = 0.0f; z < 1.0f; z+= 1.0f / gridSize) { //z
        for(float y = 0.0f; y < 1.0f; y += 1.0f / gridSize) { //y
            for(float x = 0.0f; x < 1.0f; x += 1.0f / gridSize) { //x
                cubeVertices[i++] = glm::vec3(x, y, z);
            }
        }
    }

    /*GLfloat* buffer = ParticleGpuLoader::bufferGPUParticles(data2, 0, data2->getNumberParticles());
    VAO = ParticleGpuLoader::loadParticlesToGpu(buffer, data2->getNumberParticles());*/

    GLfloat* buffer = ParticleGpuLoader::bufferGPUParticles(cubeVertices, arraySize);
    VAO = ParticleGpuLoader::loadParticlesToGpu(buffer, arraySize);

    for(auto i = 0; i < 1000; i += 3) {
        //std::cout<<cubeVertices[i].x<<" "<<cubeVertices[i].y<<" "<<cubeVertices[i].z<<std::endl;
        std::cout<<buffer[i]<<" "<<buffer[i + 1]<<" "<<buffer[i + 2]<<std::endl;
    }

    auto positionDensityData = data->getTimestep(0)->getPositionDensity();

    /*for(auto i = 0; i < 250000; i++) {
        std::cout<<positionDensityData[i].x<<" "<<positionDensityData[i].y<<" "<<positionDensityData[i].z<<" "<<positionDensityData[i].w<<std::endl;
    }*/

    GLint bla = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &bla);

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    auto height = data->getNumberParticles() / bla;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, bla, height, 0, GL_RGBA, GL_FLOAT, positionDensityData);// GL_RGBA aka position + density



    /*auto texData = new glm::vec4[bla * height];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, texData);
    for(auto i = 0; i < bla * height; i++) {
        std::cout<<texData[i].x<<" "<<texData[i].y<<" "<<texData[i].z<<" "<<texData[i].w<<std::endl;
    }*/

    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);


    glPointSize(1);
    std::cout << "Log> Initalization done" << std::endl;
}

void mainLoop() {
    doMovement(camera, window);
    window->calculateFPS();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader->use();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f));
    glm::mat4 view;
    model = camera->GetViewMatrix();
    glm::mat4 projection;
    projection = glm::perspective(camera->Zoom, window->getWidth()/window->getHeight(), 0.1f, 10.0f);

    // Pass the matrices to the shader
	auto modelLocation = glGetUniformLocation(ourShader->Program, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(ourShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(ourShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform1i(glGetUniformLocation(ourShader->Program, "size"), 250000);
    glUniform1f(glGetUniformLocation(ourShader->Program, "maxDistance"), 0.5f);


    GLint bla = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &bla);
    glUniform1i(glGetUniformLocation(ourShader->Program, "width"), bla);

    GLuint samplerLocation = glGetUniformLocation(ourShader->Program, "values");
    glUniform1i(samplerLocation, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1000);
    glBindVertexArray(0);
    glutSwapBuffers();
}

// Moves/alters the camera positions based on user input
void doMovement(Camera* camera, WindowHandler* wHandler) {
	if(wHandler->getKey('w')) {
		camera->ProcessKeyboard(FORWARD, static_cast<float>(wHandler->getDeltaTime()));
	}
	if(wHandler->getKey('s')) {
		camera->ProcessKeyboard(BACKWARD, static_cast<float>(wHandler->getDeltaTime()));
	}
	if(wHandler->getKey('a')) {
		camera->ProcessKeyboard(LEFT, static_cast<float>(wHandler->getDeltaTime()));
	}
	if(wHandler->getKey('d')) {
		camera->ProcessKeyboard(RIGHT, static_cast<float>(wHandler->getDeltaTime()));
	}
    if(wHandler->getKey('n')) {
        camera->ProcessKeyboard(UP, static_cast<float>(wHandler->getDeltaTime()));
    }
    if(wHandler->getKey('m')) {
        camera->ProcessKeyboard(DOWN, static_cast<float>(wHandler->getDeltaTime()));
    }

}

