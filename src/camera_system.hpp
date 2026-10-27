#pragma once

#include "config.h"

class CameraSystem
{
    public:
        void update(float forwardInput, float rightInput, float deltaTime);
        const glm::mat4& CameraSystem::getViewMatrix() const;
    private:
        float sensitivity_ = 2.5f;

        glm::vec3 position_ = glm::vec3(0, 0, 3);
        glm::vec3 front_ = glm::vec3(0, 0, -1);   // Negative Z direction
        glm::vec3 up_ = glm::vec3(0, 1, 0);
        glm::vec3 right_ = glm::normalize(glm::cross(up_, front_));

        glm::mat4 view_ = glm::lookAt(position_, position_ + front_, up_);
};