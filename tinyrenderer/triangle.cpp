#include <algorithm>

#include "triangle.hpp"

Triangle::Triangle() {}

Triangle::Triangle(const Vec3f p0, const Vec3f p1, const Vec3f p2) : p0(p0), p1(p1), p2(p2) {}

std::pair<Vec2i, Vec2i> Triangle::bounding_box(int screen_width, int screen_height) const
{
    const int bbox_x0 = std::max(0.f, std::min({p0.x, p1.x, p2.x})),
              bbox_x1 = std::min(static_cast<float>(screen_width) - 1, std::max({p0.x, p1.x, p2.x})),
              bbox_y0 = std::max(0.f, std::min({p0.y, p1.y, p2.y})),
              bbox_y1 = std::min(static_cast<float>(screen_height) - 1, std::max({p0.y, p1.y, p2.y}));

    return std::make_pair(Vec2i(bbox_x0, bbox_y0), Vec2i(bbox_x1, bbox_y1));
}

float Triangle::scale_barycentric(int component_idx, const Vec3f &barycentric) const
{
    const Vec3f stacked_components(p0.raw[component_idx], p1.raw[component_idx], p2.raw[component_idx]);
    return barycentric * stacked_components;
}

Vec3f &Triangle::operator[](size_t idx)
{
    switch (idx)
    {
    case 0:
        return p0;
    case 1:
        return p1;
    case 2:
        return p2;
    default:
        throw std::runtime_error("Invalid triangle index");
    }
}
