#include "main.h"
#include "DataImporter.h"
#include "DataSet.h"
#include "WindowHandler.h"
#include "DataSet.h"
#include "Camera.h"
#include "Shader.h"
#include "Timestep.h"
#include "ParticleGpuLoader.h"
#include "MarchingCubes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>
#include <iostream>


WindowHandler* window;
Camera* camera;
GLuint VBO, VAO;
Shader* ourShader;
DataSet* data;

int main(int argc, char* argv[]) {
	auto data = DataImporter::load("/home/nils/Downloads/drop.dat");
	//auto data = DataImporter::load("/home/nils/Downloads/six_iterdfdations.dat");

    auto firstTimestep = data->getTimestep(0);

    auto algorithm = MarchingCubes(0.3f);
    algorithm.calculate(firstTimestep);

    //Window Initialisation
    /*window = new WindowHandler(800, 600);
    window->initWindow(argc, argv, &init, &mainLoop);*/

    algorithm.draw();

    return 0;
}

void init() {
    std::cout << "Log> Render initialization running" << std::endl;
    camera = new Camera(glm::vec3(0.5f, 0.4f, 1.7f));
    window->setCamera(camera);
    ourShader = new Shader("shader/basic.vert", "shader/basic.frag");

    GLfloat* vertices = ParticleGpuLoader::bufferGPUParticles(data, 0, data->getNumberParticles());

    VAO = ParticleGpuLoader::loadParticlesToGpu(vertices, data->getNumberParticles());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1000000);
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

