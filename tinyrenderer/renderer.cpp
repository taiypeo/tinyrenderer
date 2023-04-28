#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "renderer.hpp"

Vec3f barycentric_coords(const Vec3f &p, const Triangle &triangle)
{
    const Vec3f p10 = triangle.p1 - triangle.p0,
                p20 = triangle.p2 - triangle.p0,
                p0p = triangle.p0 - p;

    const Vec3f top_row(p10.x, p20.x, p0p.x),
        bottom_row(p10.y, p20.y, p0p.y);
    const Vec3f cross = top_row ^ bottom_row;
    if (std::abs(cross.z) < 1)
    {
        return Vec3f(-1, 1, 1);
    }

    return Vec3f(
        1 - (cross.x + cross.y) / cross.z, cross.x / cross.z, cross.y / cross.z);
}

float get_illumination(const Vec3f &light, const Vec3f &p0, const Vec3f &p1)
{
    const Vec3f normal = p0 ^ p1;
    return light * normal / (light.norm() * normal.norm());
}

Renderer::Renderer(
    sf::Image &screen_,
    Model &model_,
    const Vec3f light_) : screen(screen_),
                          model(model_),
                          screen_width(screen_.getSize().x),
                          screen_height(screen_.getSize().y),
                          texture_width(model_.texture.getSize().x),
                          texture_height(model_.texture.getSize().y),
                          light(light_)
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
            Vec3f pixel(x, y, 0.);
            const Vec3f barycentric = barycentric_coords(pixel, triangle);
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

Vec3f Renderer::world_to_screen(const Vec3f &vec)
{
    return Vec3f(
        int((vec.x + 1.) * screen_width / 2. + .5),
        int((vec.y + 1.) * screen_height / 2. + .5),
        vec.z);
}

void Renderer::draw()
{
    for (size_t i = 0; i < model.faces.size(); ++i)
    {
        const Triangle &world_coords = model.faces[i],
                       &texture_coords = model.textures[i],
                       screen_coords(
                           world_to_screen(world_coords.p0),
                           world_to_screen(world_coords.p1),
                           world_to_screen(world_coords.p2));

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
