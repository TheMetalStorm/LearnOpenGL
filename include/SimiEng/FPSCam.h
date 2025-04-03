#pragma once
#include <iostream>

#include "SimiEng/Camera.h"
namespace SimiEng
{
    class /*SIMIENG_API*/ FPSCam : public SimiEng::Camera {
    public:
        using Camera::Camera;
        void ProcessMouseScroll(float yoffset) override;
        void ProcessKeyboard(SimiEng::CameraMovement direction, float deltaTime) override;
    };
}