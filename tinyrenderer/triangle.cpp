#include <algorithm>
#include <stdexcept>

#include "triangle.hpp"

Triangle::Triangle() {}

Triangle::Triangle(const FloatVector p0, const FloatVector p1, const FloatVector p2) : p0(p0), p1(p1), p2(p2) {}

std::pair<IntVector, IntVector> Triangle::bounding_box(int screen_width, int screen_height) const
{
    const int bbox_x0 = std::max(0.f, std::min({p0.x, p1.x, p2.x})),
              bbox_x1 = std::min(static_cast<float>(screen_width) - 1, std::max({p0.x, p1.x, p2.x})),
              bbox_y0 = std::max(0.f, std::min({p0.y, p1.y, p2.y})),
              bbox_y1 = std::min(static_cast<float>(screen_height) - 1, std::max({p0.y, p1.y, p2.y}));

    return std::make_pair(IntVector(bbox_x0, bbox_y0), IntVector(bbox_x1, bbox_y1));
}

float Triangle::scale_barycentric(size_t component_idx, const FloatVector &barycentric) const
{
    const FloatVector stacked_components(p0.at(component_idx), p1.at(component_idx), p2.at(component_idx));
    return barycentric * stacked_components;
}
