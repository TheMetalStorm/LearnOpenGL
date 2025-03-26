
export module fps_cam;

import camera;

export namespace fps_cam {

    class FPSCam : public camera::Camera {
    public:
        using Camera::Camera;

        void ProcessMouseScroll(float yoffset) override
        {
            return;
        }

        void ProcessKeyboard(camera::Camera_Movement direction, float deltaTime) override
        {
            const glm::vec3 forward = glm::normalize(glm::vec3(
                cos(glm::radians(Yaw)),
                0.0f, sin(glm::radians(Yaw))
            ));

            const glm::vec3 right = glm::normalize(glm::cross(forward, WorldUp));

            SetMovementSpeed();
            const float velocity = MovementSpeed * deltaTime;

            switch (direction) {
            case camera::Camera_Movement::FORWARD:  Position += forward * velocity; break;
            case camera::Camera_Movement::BACKWARD: Position -= forward * velocity; break;
            case camera::Camera_Movement::LEFT:     Position -= right * velocity;   break;
            case camera::Camera_Movement::RIGHT:    Position += right * velocity;   break;
            }
        }
    };
}