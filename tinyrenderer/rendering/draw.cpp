#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "rendering/draw.hpp"

FloatVector barycentric_coords(const FloatVector &p, const Triangle &triangle)
{
    const FloatVector p10 = triangle.p1 - triangle.p0,
                      p20 = triangle.p2 - triangle.p0,
                      p0p = triangle.p0 - p;

    const FloatVector top_row(p10.x, p20.x, p0p.x),
        bottom_row(p10.y, p20.y, p0p.y);
    const FloatVector cross = top_row ^ bottom_row;
    if (std::abs(cross.z) < 1)
    {
        return FloatVector(-1.f, 1.f, 1.f);
    }

    return FloatVector(
        1 - (cross.x + cross.y) / cross.z, cross.x / cross.z, cross.y / cross.z);
}

void draw_triangle(
    sf::Image &screen,
    const Triangle &triangle,
    std::vector<std::vector<float>> &zbuf,
    Shader &shader)
{
    const auto screen_size = screen.getSize();
    const auto bbox = triangle.bounding_box(screen_size.x, screen_size.y);
    for (int x = bbox.first.x; x <= bbox.second.x; ++x)
    {
        for (int y = bbox.first.y; y <= bbox.second.y; ++y)
        {
            FloatVector pixel(x, y, 0.);
            const FloatVector barycentric = barycentric_coords(pixel, triangle);
            if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0)
            {
                continue;
            }

            pixel.z = triangle.scale_barycentric(VectorComponent::Z, barycentric);
            if (pixel.z < zbuf[x][y])
            {
                continue;
            }

            sf::Color color = sf::Color::Black;
            if (shader.fragment(barycentric, color))
            {
                continue;
            }

            zbuf[x][y] = pixel.z;
            screen.setPixel(x, y, color);
        }
    }
}

void draw_line(
    sf::Image &screen,
    int x0,
    int y0,
    int x1,
    int y1,
    const sf::Color &color)
{
    bool transpose = false;
    if (std::abs(x1 - x0) < std::abs(y1 - y0))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        transpose = true;
    }

    if (x1 < x0)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    const int dy = y1 - y0, dx = x1 - x0;
    int two_error = 0, y = y0, err_plus_delta = 2 * std::abs(dy), err_minus_delta = 2 * dx;
    for (int x = x0; x <= x1; ++x)
    {
        if (transpose)
        {
            screen.setPixel(y, x, color);
        }
        else
        {
            screen.setPixel(x, y, color);
        }

        two_error += err_plus_delta;
        if (two_error > dx)
        {
            y += y1 > y0 ? 1 : -1;
            two_error -= err_minus_delta;
        }
    }
}
