#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "linalg.hpp"

struct Camera
{
    FloatVector eye, center, up;

    Camera(const FloatVector &eye, const FloatVector &center, const FloatVector &up);
};

#endif
