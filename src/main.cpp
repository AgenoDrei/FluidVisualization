#include "main.h"
#include "DataImporter.h"
#include "WindowHandler.h"
#include "Camera.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>

WindowHandler* window;
Camera* camera;
GLuint VBO, VAO;
Shader* ourShader;

int main(int argc, char* argv[]) {
	//auto data = DataImporter::load("/home/nils/Downloads/drop.dat");
	//auto data = DataImporter::load("/home/nils/Downloads/six_iterdfdations.dat");

    //Window Initialisation
    window = new WindowHandler(800, 600);
    window->initWindow(argc, argv, &init, &mainLoop);
    return 0;
}

void init() {
    std::cout << "Log> Render initialization running" << std::endl;
    camera = new Camera(glm::vec3(0.0f, 0.0f, 2.0f));
    window->setCamera(camera);

    ourShader = new Shader("shader/basic.vert", "shader/basic.frag");
    GLfloat vertices[] {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);     // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));     // TexCoord attribute
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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
    projection = glm::perspective(camera->Zoom, window->getWidth()/window->getHeight(), 1.0f, 100.0f);

    // Pass the matrices to the shader
    glUniformMatrix4fv(glGetUniformLocation(ourShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(ourShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(ourShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glutSwapBuffers();
}

// Moves/alters the camera positions based on user input
void doMovement(Camera* camera, WindowHandler* wHandler)
{
    if(wHandler->getKey(87) || wHandler->getKey(119))
        camera->ProcessKeyboard(FORWARD, wHandler->getDeltaTime());
    if(wHandler->getKey(83) || wHandler->getKey(115))
        camera->ProcessKeyboard(BACKWARD, wHandler->getDeltaTime());
    if(wHandler->getKey(65) || wHandler->getKey(97))
        camera->ProcessKeyboard(LEFT, wHandler->getDeltaTime());
    if(wHandler->getKey(68) || wHandler->getKey(100))
        camera->ProcessKeyboard(RIGHT, wHandler->getDeltaTime());
}

