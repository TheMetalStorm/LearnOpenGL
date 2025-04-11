#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimiEng/SimiEng.h"

namespace SimiEng
{
    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum class CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    // Default camera values
    const float DEFAULT_YAW = -90.0f;
    const float DEFAULT_PITCH = 0.0f;
    const float DEFAULT_SPEED = 2.5f;
    const float DEFAULT_FAST_SPEED = 10.0f;
    const float DEFAULT_SENSITIVITY = 0.1f;
    const float DEFAULT_ZOOM = 45.0f;
    const glm::vec3 DEFAULT_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 DEFAULT_UP = glm::vec3(0.0f, 1.0f, 0.0f);


    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class SIMIENG_API Camera
    {
    public:
         
        // Public variables (could use getters/setters instead)
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        float Yaw;
        float Pitch;
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;
        bool MoveFast;

        Camera(glm::vec3 position , glm::vec3 up = DEFAULT_UP, float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH);
        Camera(float posX, float posY, float posZ, float upX = DEFAULT_UP.x, float upY= DEFAULT_UP.y, float upZ = DEFAULT_UP.z, float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH);
        virtual ~Camera() = default;

        glm::mat4 GetViewMatrix() const;
        void SetMovementSpeed();
        void SetFastMovement(bool fast);
        virtual void ProcessKeyboard(CameraMovement direction, float deltaTime);
        virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        virtual void ProcessMouseScroll(float yOffset);

    protected:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors() ;

    };
}