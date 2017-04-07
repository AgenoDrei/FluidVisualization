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
#define SPECIAL_KEYS_SIZE 1024

class Configuration;

class WindowHandler {
public:
    WindowHandler(unsigned int windowWidth, unsigned int windowHeight);
    ~WindowHandler();
    void initWindow(int argc, const char* argv[], void (*init)(Configuration*),void (*mainLoop)(),Configuration*);
    void setCamera(Camera* camera);
    void resize(int newWidth, int newHeight) const;
    void processKeyboard(bool pressed, unsigned char key, int x, int y);
    void processMouse(int button, int state, int x, int y);
    void onSpecialKey(int key, int x, int y);
    void onSpecialKeyUp(int key, int x, int y);
    static void onSpecialKeyStatic(int key, int x, int y);
    static void onSpecialKeyUpStatic(int key, int x, int y);

    void onMotion(int x, int y);
    static void onMotionStatic(int x, int y);

    bool getSpecialKey(int key);

    static WindowHandler* instance;
    bool getKey(char key);
    bool getKeyDebounce(char key);
    double getDeltaTime() const;
	float getWidth() const;
	float getHeight() const;
    int calculateFPS();

private:
    unsigned int width, height;
    Camera* camera;

    bool keys[KEYS_SIZE];
    bool keysDebounce[KEYS_SIZE];

    bool specialKeys[SPECIAL_KEYS_SIZE];
    long currentTime, lastTime, deltaTime;
    volatile bool firstMouse = true;
    float lastX, lastY;
};


