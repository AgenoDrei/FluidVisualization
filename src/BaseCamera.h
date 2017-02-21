#pragma once

#include <glm/detail/type_mat.hpp>

class WindowHandler;

class BaseCamera {
public:
    virtual glm::mat4 GetViewMatrix() const = 0;
    virtual glm::mat4 GetProjectonMatrix(WindowHandler* wHandler, float nearPlane, float farPlane) const = 0;
};



