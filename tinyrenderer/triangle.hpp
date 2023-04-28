#ifndef __TRIANGLE_HPP__
#define __TRIANGLE_HPP__

#include <utility>

#include "../third-party/geometry.h"

enum VectorComponent
{
    X,
    Y,
    Z
};

struct Triangle
{
    Vec3f p0, p1, p2;

    Triangle();
    Triangle(const Vec3f p0, const Vec3f p1, const Vec3f p2);
    std::pair<Vec2i, Vec2i> bounding_box(int screen_width, int screen_height) const;
    float scale_barycentric(int component_idx, const Vec3f &barycentric) const;
    Vec3f &operator[](size_t idx);
};

#endif
