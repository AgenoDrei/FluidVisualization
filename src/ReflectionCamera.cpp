#include <glm/ext.hpp>
#include "ReflectionCamera.h"
#include "WindowHandler.h"

ReflectionCamera::ReflectionCamera(BaseCamera* camera, glm::vec3 normal) :
    _camera(camera),
    _normal(normal) {
}

glm::mat4 ReflectionCamera::GetViewMatrix() const  {
    //return _camera->GetViewMatrix();
    auto a = getReflectionMatrix() * _camera->GetViewMatrix();
    return a;
}

glm::mat4 ReflectionCamera::getReflectionMatrix() const {
    /*            | 1-2Nx2   -2NxNy  -2NxNz  -2NxD |
    Mreflection = |  -2NxNy 1-2Ny2   -2NyNz  -2NyD |
                  |  -2NxNz  -2NyNz 1-2Nz2   -2NzD |
                  |    0       0       0       1   |
     */
    float d = 0.0f;
    return glm::mat4(1 - 2 * _normal.x,  -2  * _normal.x * _normal.y, -2  * _normal.x * _normal.z, -2 * _normal.x * d,
                     -2 * _normal.x * _normal.y, 1 - 2 * _normal.y, -2 * _normal.y * _normal.z, -2 * _normal.y * d,
                     -2 * _normal.x * _normal.z, 2 * _normal.y * _normal.z, 1 - 2 * _normal.z, -2 * _normal.z * d,
                     0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 ReflectionCamera::GetProjectonMatrix(WindowHandler* wHandler, float nearPlane, float farPlane) const {
    return glm::perspective(1.0f, wHandler->getWidth()/wHandler->getHeight(), nearPlane, farPlane);
};

glm::vec3 ReflectionCamera::getFront() const {
    return _camera->getFront();
}
