#pragma once

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "BaseCamera.h"
#include "TextureSlicingRenderer.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class WindowHandler;

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  0.5f;
const GLfloat SENSITIVTY =  0.25f;
const GLfloat ZOOM       =  45.0f;


class Camera : public BaseCamera {
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

    ~Camera();

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    virtual glm::mat4 GetViewMatrix() const override;
    // Processes input received from any keyboard-like input system.
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset);

    virtual glm::mat4 GetProjectonMatrix(WindowHandler* wHandler, float nearPlane, float farPlane) const override;

    glm::vec3 getFront() const override;

    Camera* getCopy();
private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();
};