#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "renderer.hpp"

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

float get_illumination(const FloatVector &light, const FloatVector &p0, const FloatVector &p1)
{
    const FloatVector normal = p0 ^ p1;
    return light * normal / (light.norm() * normal.norm());
}

Renderer::Renderer(
    sf::Image &screen,
    Model &model,
    float camera_z,
    const FloatVector light) : screen(screen),
                               model(model),
                               screen_width(screen.getSize().x),
                               screen_height(screen.getSize().y),
                               texture_width(model.texture.getSize().x),
                               texture_height(model.texture.getSize().y),
                               light(light),
                               camera_z(camera_z)
{
    zbuf = std::vector<std::vector<float>>(
        screen_width,
        std::vector<float>(screen_height, -std::numeric_limits<float>::max()));
}

void Renderer::draw_triangle(
    const Triangle &triangle,
    const Triangle &texture_triangle,
    float illumination)
{
    const auto bbox = triangle.bounding_box(screen_width, screen_height);
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

            const float texture_x = texture_triangle.scale_barycentric(VectorComponent::X, barycentric) * texture_width,
                        texture_y = texture_triangle.scale_barycentric(VectorComponent::Y, barycentric) * texture_height;

            sf::Color color = model.texture.getPixel(texture_x, texture_y);
            color.r *= illumination;
            color.g *= illumination;
            color.b *= illumination;

            if (pixel.z > zbuf[x][y])
            {
                zbuf[x][y] = pixel.z;
                screen.setPixel(x, y, color);
            }
        }
    }
}

void Renderer::draw_line(int x0, int y0, int x1, int y1, const sf::Color &color)
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

void Renderer::draw()
{
    Matrix vp = Matrix::viewport(screen_width / 8, screen_height / 8, screen_width * 3 / 4, screen_height * 3 / 4),
           proj = Matrix::projection(camera_z);

    for (size_t i = 0; i < model.faces.size(); ++i)
    {
        const Triangle &world_coords = model.faces[i],
                       &texture_coords = model.textures[i],
                       screen_coords(
                           (vp * proj * Matrix(world_coords.p0)).to_vector(),
                           (vp * proj * Matrix(world_coords.p1)).to_vector(),
                           (vp * proj * Matrix(world_coords.p2)).to_vector());

        const float illumination = get_illumination(
            light,
            world_coords.p2 - world_coords.p0,
            world_coords.p1 - world_coords.p0);

        if (illumination > 0.f)
        {
            draw_triangle(
                screen_coords,
                texture_coords,
                illumination);
        }
    }

    screen.flipVertically();
}
