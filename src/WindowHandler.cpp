//
// Created by simon on 24.10.16.
//

#include "WindowHandler.h"

WindowHandler* WindowHandler::instance = nullptr;

WindowHandler::WindowHandler(unsigned int windowWidth, unsigned int windowHeight) {
    WindowHandler::instance = this;
    width = windowWidth;
    height = windowHeight;
    std::cout << "Log> Created window handler" << std::endl;
}

WindowHandler::~WindowHandler() {
    std::cout << "Log> Destroyed window handler" << std::endl;
}

void WindowHandler::initWindow(int argc, char* argv[], void (*init)(), void (*mainLoop)()) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE |
                        GLUT_RGBA);
    //glutInitContextVersion (3, 3);
    glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);
    //glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
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
    glViewport(0, 0, width, height);

    // Setup some OpenGL options
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(debugOutput), nullptr);

    currentTime = 0;

    (*init)();
    glutCloseFunc(onShutdown);
    glutKeyboardFunc(onKeyDown); //Keyboard
    glutKeyboardUpFunc(onKeyUp);
    glutIgnoreKeyRepeat(true);
    glutPassiveMotionFunc(onMouse); //Mouse
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
        if(pressed)
            keys[key] = true;
        else if(!pressed)
            keys[key] = false;
    }
}

void WindowHandler::processMouse(int x, int y) {
    //std::cout << "Mouse Pos (" << x << " | " << y << ")" << std::endl;
    if(firstMouse){
        glutWarpPointer(width/2, height/2);
        lastX = x = width/2;
        lastY = y = height/2;
        firstMouse = false;
    }
    auto xoffset = x - lastX;
    auto yoffset = lastY - y;  // Reversed since y-coordinates go from bottom to left

    lastX = x;
    lastY = y;

    camera->ProcessMouseMovement(static_cast<GLfloat>(xoffset), static_cast<GLfloat>(yoffset), true);
}

bool WindowHandler::getKey(int key) {
    return keys[key];
}

float WindowHandler::getWidth() const {
    return static_cast<float>(width);
}
float WindowHandler::getHeight() const {
    return static_cast<float>(height);
}

//TODO Include correct FPS Calculation
void WindowHandler::calculateFPS() {
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}

double WindowHandler::getDeltaTime() const {
    return deltaTime / 1000.0f;
}

/****************************************************************
 **** WRAPPER FUNCTIONS FOR FREEGLUT ****************************
 ****************************************************************/
void onResize(int nw, int nh) {
    if(WindowHandler::instance != nullptr)
        WindowHandler::instance->resize(nw, nh);
}

void onKeyDown(unsigned char key, int x, int y) {
    if(WindowHandler::instance != nullptr)
        WindowHandler::instance->processKeyboard(true, key, x, y);
}

void onKeyUp(unsigned char key, int x, int y) {
    if(WindowHandler::instance != nullptr)
        WindowHandler::instance->processKeyboard(false, key, x, y);
}

void onMouse(int x, int y) {
    if(WindowHandler::instance != nullptr)
        WindowHandler::instance->processMouse(x, y);
}

void onShutdown() {
    delete WindowHandler::instance;
    std::cout << "Log> glut shutdown successfull" << std::endl;
}

void debugOutput(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {
    std::cout<< "ERROR> Source: "<< source << " Message: "<<message<<std::endl;
}
