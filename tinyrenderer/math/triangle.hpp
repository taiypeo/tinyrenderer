#ifndef __TRIANGLE_HPP__
#define __TRIANGLE_HPP__

#include <utility>

#include "math/linalg.hpp"

struct Triangle
{
    FloatVector p0, p1, p2;

    Triangle();
    Triangle(const FloatVector p0, const FloatVector p1, const FloatVector p2);
    std::pair<IntVector, IntVector> bounding_box(int screen_width, int screen_height) const;
    float scale_barycentric(size_t component_idx, const FloatVector &barycentric) const;
    const FloatVector &at(size_t idx) const;
    FloatVector &operator[](size_t idx);
};

#endif
