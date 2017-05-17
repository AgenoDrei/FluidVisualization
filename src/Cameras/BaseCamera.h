#pragma once

#include <glm/detail/type_mat.hpp>
#include <glm/vec3.hpp>

class WindowHandler;

class BaseCamera {
public:
    virtual ~BaseCamera();

    virtual glm::mat4 GetViewMatrix() const = 0;
    virtual glm::mat4 GetProjectonMatrix(WindowHandler* wHandler, float nearPlane, float farPlane) const = 0;

    virtual glm::vec3 getFront() const = 0;
};



