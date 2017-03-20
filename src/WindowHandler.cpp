//
// Created by simon on 24.10.16.
//

#include "WindowHandler.h"
#include <iostream>
#include <cstring>

WindowHandler* WindowHandler::instance = nullptr;
double lastTime = 0.0;

WindowHandler::WindowHandler(unsigned int windowWidth, unsigned int windowHeight) {
    WindowHandler::instance = this;
    std::memset(keys, 0, sizeof(bool) * KEYS_SIZE);
    width = windowWidth;
    height = windowHeight;
    lastTime = glutGet(GLUT_ELAPSED_TIME);
    std::cout << "Log> Created window handler" << std::endl;
}

WindowHandler::~WindowHandler() {
    std::cout << "Log> Destroyed window handler" << std::endl;
}

void WindowHandler::initWindow(int argc, char* argv[], void (*init)(InitParameter*), void (*mainLoop)(), InitParameter* parameter) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE |
                        GLUT_RGBA);
    glutInitContextVersion (3, 3);
    glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitWindowSize(width, height);
    glutCreateWindow("Fluid Simulation");

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
	if(GLEW_OK != err) {
		std::cout << "Log> Glew initalization failed" << std::endl;
		throw glewGetErrorString(err);
	}
	if(GLEW_VERSION_3_3) {
		std::cout << "Log> Driver supports OpenGL 3.3" << std::endl;
	}
	if(GLEW_VERSION_4_1) {
		std::cout << "Log> Driver supports OpenGL 4.1" << std::endl;
	}
    if(GLEW_VERSION_4_4) {
        std::cout << "Log> Driver supports OpenGL 4.4" << std::endl;
    }
    glViewport(0, 0, width, height);

    // Setup some OpenGL options
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(debugOutput), nullptr);

    currentTime = 0;

    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutIgnoreKeyRepeat(0);

    (*init)(parameter);
    glutCloseFunc(onShutdown);
    glutKeyboardFunc(onKeyDown); //Keyboard
    glutKeyboardUpFunc(onKeyUp);
    glutIgnoreKeyRepeat(false);
    glutMouseFunc(onMouse);
    glutDisplayFunc(mainLoop); //Rendering Loop
    glutIdleFunc(mainLoop);
    glutReshapeFunc(onResize); //Resize


    glutMainLoop(); //Start Rendering Loop
}

void WindowHandler::setCamera(Camera *camera) {
    this->camera = camera;
}

void WindowHandler::resize(int newWidth, int newHeight) const {
    glutReshapeWindow(width, height);
}

void WindowHandler::processKeyboard(bool pressed, unsigned char key, int x, int y) {
    //std::cout << "Log> Key pressed: " << pressed << " - Specifc key: " << (int)key << std::endl;
    if (key >= 0 && key < 1024){
        keys[key] = pressed;
    }
}

void WindowHandler::processMouse(int button, int state, int x, int y) {
    //std::cout << "Mouse Pos (" << x << " | " << y << ")" << std::endl;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && firstMouse) {
        firstMouse = false;
        lastX = x;
        lastY = y;
    } else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        firstMouse = true;
        auto xoffset = x - lastX;
        auto yoffset = lastY - y;
        camera->ProcessMouseMovement(static_cast<GLfloat>(xoffset), static_cast<GLfloat>(yoffset), true);
    }
}

bool WindowHandler::getKey(char key) {
    return keys[static_cast<int>(key)] != 0;
}

float WindowHandler::getWidth() const {
    return static_cast<float>(width);
}
float WindowHandler::getHeight() const {
    return static_cast<float>(height);
}

int WindowHandler::calculateFPS() {
    currentTime = glutGet(GLUT_ELAPSED_TIME);       // this NEEDS to get called if camera should be movable...
    deltaTime = currentTime - this->lastTime;
    //std::cout << "FPS: " << (int)((1000.0 / deltaTime) + 0.5) << std::endl;
    lastTime = currentTime;
    return (int)((1000.0 / deltaTime) + 0.5);
}

double WindowHandler::getDeltaTime() const {
    return deltaTime / 1000.0f;
}

/****************************************************************
 **** WRAPPER FUNCTIONS FOR FREEGLUT ****************************
 ****************************************************************/
void onResize(int nw, int nh) {
    if(WindowHandler::instance != nullptr) {
        WindowHandler::instance->resize(nw, nh);
    }
}

void onKeyDown(unsigned char key, int x, int y) {
    //std::cout << "Key down: " << key << std::endl;
    if(WindowHandler::instance != nullptr) {
        WindowHandler::instance->processKeyboard(true, key, x, y);
    }
}

void onKeyUp(unsigned char key, int x, int y) {
    //std::cout << "Key up: " << key << std::endl;
    if(WindowHandler::instance != nullptr) {
        WindowHandler::instance->processKeyboard(false, key, x, y);
    }
}

void onMouse(int button, int state, int x, int y) {
    if(WindowHandler::instance != nullptr) {
        WindowHandler::instance->processMouse(button, state, x, y);
    }
}

void onShutdown() {
    delete WindowHandler::instance;
    std::cout << "Log> glut shutdown successfull" << std::endl;
}

void debugOutput(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {
    std::cout<< "GlLog> Source: "<< source << " Message: "<<message<<std::endl;
}
