
export module camera;

import "glad/glad.h";
import "glm/glm.hpp";
import "glm/gtc/matrix_transform.hpp";

export namespace camera {

    // Defines several possible options for camera movement
    enum class Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    // Default camera values
    constexpr float YAW = -90.0f;
    constexpr float PITCH = 0.0f;
    constexpr float SPEED = 2.5f;
    constexpr float FASTSPEED = 10.0f;
    constexpr float SENSITIVITY = 0.1f;
    constexpr float ZOOM = 45.0f;

    // An abstract camera class that processes input and calculates matrices
    class Camera {
    public:
        // camera Attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        // euler Angles
        float Yaw;
        float Pitch;

        // camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;
        bool moveFast;

        // constructors
        Camera(glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = YAW,
            float pitch = PITCH);

        Camera(float posX, float posY, float posZ,
            float upX, float upY, float upZ,
            float yaw, float pitch);

        virtual ~Camera() = default;

        // returns the view matrix
        glm::mat4 GetViewMatrix();

        void SetMovementSpeed();

        // input processing methods
        virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime);
        virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        virtual void ProcessMouseScroll(float yoffset);

    protected:
        // calculates the front vector from the Camera's Euler Angles
        void updateCameraVectors();
    };
}

// Implementation can be in the same file or separated into an implementation partition
module :private;  // private module fragment for implementation

namespace camera {
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM),
        moveFast(false) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM),
        moveFast(false) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 Camera::GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void Camera::SetMovementSpeed() {
        MovementSpeed = moveFast ? FASTSPEED : SPEED;
    }

    void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        SetMovementSpeed();
        float velocity = MovementSpeed * deltaTime;
        switch (direction) {
        case Camera_Movement::FORWARD:  Position += Front * velocity; break;
        case Camera_Movement::BACKWARD: Position -= Front * velocity; break;
        case Camera_Movement::LEFT:     Position -= Right * velocity; break;
        case Camera_Movement::RIGHT:   Position += Right * velocity; break;
        }
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            Pitch = glm::clamp(Pitch, -89.0f, 89.0f);
        }

        updateCameraVectors();
    }

    void Camera::ProcessMouseScroll(float yoffset) {
        Zoom -= yoffset;
        Zoom = glm::clamp(Zoom, 1.0f, 45.0f);
    }

    void Camera::updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
}