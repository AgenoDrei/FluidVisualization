#include "main.h"
#include "DataImporter.h"
#include "DataSet.h"
#include "WindowHandler.h"
#include "Camera.h"
#include "Shader.h"
#include "Timestep.h"
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
	data = DataImporter::load("/home/simon/Downloads/drop.dat");
	//auto data = DataImporter::load("/home/nils/Downloads/six_iterdfdations.dat");

    //Window Initialisation
    window = new WindowHandler(800, 600);
    window->initWindow(argc, argv, &init, &mainLoop);
    return 0;
}

void init() {
    std::cout << "Log> Render initialization running" << std::endl;
    camera = new Camera(glm::vec3(0.0f, 2.0f, -3.0f));
    window->setCamera(camera);

    GLfloat vertices[data->getNumberParticles()*4];
    Timestep* step = data->getTimestep(0);
    uint32_t index = 0;
    for(auto j = 0; j < data->getNumberParticles(); j++) {
        Particle particle = step->getParticle(j);
        vertices[index] = particle.position.x;
        vertices[index + 1] = particle.position.y;
        vertices[index + 2] = particle.position.z;
        vertices[j + 4] = particle.density;
        index+=4;

    }

    std::cout << vertices << std::endl;

    ourShader = new Shader("shader/basic.vert", "shader/basic.frag");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));     // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));     // TexCoord attribute
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

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

    std::cout << "Camera postion: " << glm::to_string(camera->Position) << std::endl;
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

