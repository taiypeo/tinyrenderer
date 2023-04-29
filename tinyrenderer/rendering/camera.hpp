#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "math/linalg.hpp"

struct Camera
{
    FloatVector eye, center, up;

    Camera(const FloatVector &eye, const FloatVector &center, const FloatVector &up);
};

#endif
