//
// Created by simon on 24.10.16.
//

#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Cameras/Camera.h"

void debugOutput(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);
void onShutdown();
void onResize(int nw, int nh);
void onKeyDown(unsigned char key, int x, int y);
void onKeyUp(unsigned char key, int x, int y);
void onMouse(int button, int state, int x, int y);

#define KEYS_SIZE 1204

class InitParameter;

class WindowHandler {
public:
    WindowHandler(unsigned int windowWidth, unsigned int windowHeight);
    ~WindowHandler();
    void initWindow(int argc, char* argv[], void (*init)(InitParameter*),void (*mainLoop)(),InitParameter*);
    void setCamera(Camera* camera);
    void resize(int newWidth, int newHeight) const;
    void processKeyboard(bool pressed, unsigned char key, int x, int y);
    void processMouse(int button, int state, int x, int y);

    static WindowHandler* instance;
    bool getKey(char key);
    double getDeltaTime() const;
	float getWidth() const;
	float getHeight() const;
    int calculateFPS();

private:
    unsigned int width, height;
    Camera* camera;
    bool keys[KEYS_SIZE];
    long currentTime, lastTime, deltaTime;
    bool firstMouse = true;
    int lastX, lastY;
};


