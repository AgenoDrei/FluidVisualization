#pragma once

#include <glm/glm.hpp>
#include "BaseCamera.h"

class ReflectionCamera : public BaseCamera {
private:
    BaseCamera* _camera;
    glm::vec3 _normal;

    glm::mat4 getReflectionMatrix() const;
public:
    ReflectionCamera(BaseCamera* camera, glm::vec3 normal);

    virtual glm::mat4 GetViewMatrix() const override;
    virtual glm::mat4 GetProjectonMatrix(WindowHandler* wHandler, float nearPlane, float farPlane) const override;

    virtual glm::vec3 getFront() const override;

};



