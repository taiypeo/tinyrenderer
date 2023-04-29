#include "rendering/camera.hpp"

Camera::Camera(
    const FloatVector &eye,
    const FloatVector &center,
    const FloatVector &up) : eye(eye),
                             center(center),
                             up(up) {}
