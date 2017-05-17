#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() :
    direction(glm::vec3(0.0f, 0.0f, 0.0f)) {
}

DirectionalLight::DirectionalLight(glm::vec3 direction) :
    direction(direction) {

}
