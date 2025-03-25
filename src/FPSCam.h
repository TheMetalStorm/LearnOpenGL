#pragma once
#include <iostream>

#include "Camera.h"

class FPSCam : public Camera {
public:
    using Camera::Camera;
    void ProcessMouseScroll(float yoffset) override
    {
        return;
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) override
    {

        const glm::vec3 forward = glm::normalize(glm::vec3(
            cos(glm::radians(Yaw)),
            0.0f,              sin(glm::radians(Yaw))
        ));

        const glm::vec3 right = glm::normalize(glm::cross(forward, WorldUp));

        SetMovementSpeed();
        const float velocity = MovementSpeed * deltaTime;

        switch (direction) {
            case FORWARD:  Position += forward * velocity; break;
            case BACKWARD: Position -= forward * velocity; break;
            case LEFT:     Position -= right * velocity;   break;
            case RIGHT:    Position += right * velocity;   break;
        }
    }
};