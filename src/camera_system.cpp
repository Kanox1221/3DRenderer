#include "camera_system.hpp"

void CameraSystem::update(float forwardInput, float rightInput, float deltaTime)
{
    float cameraSpeed = sensitivity_ * deltaTime;

    position_ += front_ * forwardInput * cameraSpeed + right_ * rightInput * cameraSpeed;

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
