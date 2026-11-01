#include "camera_system.hpp"

void CameraSystem::update(float forwardInput, float rightInput,
    float mouseDeltaX, float mouseDeltaY, float deltaTime)
{
    /* Rotation Update */
    yaw_   += mouseDeltaX * mouseSensitivity_;
    pitch_ -= mouseDeltaY * mouseSensitivity_;
    pitch_ = glm::clamp(pitch_, -89.0f, 89.0f);

    float yawRad = glm::radians(yaw_);
    float pitchRad = glm::radians(pitch_);

    /* Camera Axis Update */
    front_ = glm::normalize(glm::vec3(
        cos(yawRad) * cos(pitchRad),
        sin(pitchRad),
        sin(yawRad) * cos(pitchRad)
    ));

    right_ = glm::normalize(glm::cross(front_, up_));

    /* Camera Axis Update */
    float cameraSpeed = movementSpeed_ * deltaTime;
    position_ += (front_ * forwardInput + right_ * rightInput) * cameraSpeed;

    view_ = glm::lookAt(
        position_,
        position_ + front_,
        up_
    );
}

const glm::mat4& CameraSystem::getViewMatrix() const
{
    return view_;
}
