#include <iostream>
#include "SimiEng/FPSCam.h"
namespace SimiEng {

    void FPSCam::ProcessMouseScroll(float yoffset) 
    {
        return;
    }

    void FPSCam::ProcessKeyboard(SimiEng::CameraMovement direction, float deltaTime) 
    {

        const glm::vec3 forward = glm::normalize(glm::vec3(
            cos(glm::radians(Yaw)),
            0.0f, sin(glm::radians(Yaw))
        ));

        const glm::vec3 right = glm::normalize(glm::cross(forward, WorldUp));

        SetMovementSpeed();
        const float velocity = MovementSpeed * deltaTime;

        switch (direction) {
        case SimiEng::CameraMovement::FORWARD:  Position += forward * velocity; break;
        case SimiEng::CameraMovement::BACKWARD: Position -= forward * velocity; break;
        case SimiEng::CameraMovement::LEFT:     Position -= right * velocity;   break;
        case SimiEng::CameraMovement::RIGHT:    Position += right * velocity;   break;
        }
    }
}