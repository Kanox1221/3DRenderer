#pragma once

#include "config.h"

class CameraSystem
{
    public:
        void update(float forwardInput, float rightInput, float mouseDeltaX, float mouseDeltaY, float deltaTime);
        const glm::mat4& getViewMatrix() const;
    private:
        /* KNX : Camera Initial Setup */
        // Translation
        glm::vec3 position_ = glm::vec3(0, 0, 3);
        glm::vec3 front_ = glm::vec3(0, 0, -1);   // Negative Z direction
        glm::vec3 up_ = glm::vec3(0, 1, 0);
        glm::vec3 right_ = glm::normalize(glm::cross(up_, front_));

        // Rotation
        float yaw_ = -90.0f;
        float pitch_ = 0.0f;
        float movementSpeed_ = 2.5f;
        float mouseSensitivity_ = 0.1f;

        glm::mat4 view_ = glm::lookAt(position_, position_ + front_, up_);
};