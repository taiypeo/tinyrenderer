#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <limits>

#include <SFML/Graphics.hpp>

#include "../third-party/model.h"
#include "../third-party/geometry.h"

const sf::Color WHITE(255, 255, 255), RED(255, 0, 0), GREEN(0, 255, 0);

void draw_line(sf::Image &img, int x0, int y0, int x1, int y1, const sf::Color &color)
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
            img.setPixel(y, x, color);
        }
        else
        {
            img.setPixel(x, y, color);
        }

        two_error += err_plus_delta;
        if (two_error > dx)
        {
            y += y1 > y0 ? 1 : -1;
            two_error -= err_minus_delta;
        }
    }
}

Vec3f barycentric_coords(
    const Vec3f &p,
    const Vec3f &p0,
    const Vec3f &p1,
    const Vec3f &p2)
{
    const Vec3f p10 = p1 - p0,
                p20 = p2 - p0,
                p0p = p0 - p;

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

void draw_triangle(
    sf::Image &img,
    std::vector<float> &zbuf,
    Vec3f p0,
    Vec3f p1,
    Vec3f p2,
    const sf::Color &color)
{
    const auto size = img.getSize();
    const int width = size.x, height = size.y;

    const int bbox_x0 = std::max(0.f, std::min({p0.x, p1.x, p2.x})),
              bbox_x1 = std::min(static_cast<float>(width) - 1, std::max({p0.x, p1.x, p2.x})),
              bbox_y0 = std::max(0.f, std::min({p0.y, p1.y, p2.y})),
              bbox_y1 = std::min(static_cast<float>(height) - 1, std::max({p0.y, p1.y, p2.y}));

    for (int x = bbox_x0; x <= bbox_x1; ++x)
    {
        for (int y = bbox_y0; y <= bbox_y1; ++y)
        {
            Vec3f p(x, y, 0.);
            const Vec3f barycentric = barycentric_coords(p, p0, p1, p2);
            if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0)
            {
                continue;
            }

            p.z = barycentric.x * p0.z + barycentric.y * p1.z + barycentric.z * p2.z;
            if (p.z > zbuf[y * width + x])
            {
                zbuf[y * width + x] = p.z;
                img.setPixel(x, y, color);
            }
        }
    }
}

float get_illumination(const Vec3f light, const Vec3f p0, const Vec3f p1)
{
    const Vec3f normal = p0 ^ p1;
    return light * normal / (light.norm() * normal.norm());
}

void cmd_draw_head()
{
    const float width = 800, height = 800;

    sf::Image img;
    img.create(width, height, sf::Color::Black);

    Model model("obj/head.obj");

    for (size_t i = 0; i < model.nfaces(); ++i)
    {
        const std::vector<int> face = model.face(i);
        for (size_t j = 0; j < 3; ++j)
        {
            const Vec3f v0 = model.vert(face[j]), v1 = model.vert(face[(j + 1) % 3]);
            const int x0 = (v0.x + 1.) * width / 2.,
                      y0 = (v0.y + 1.) * height / 2.,
                      x1 = (v1.x + 1.) * width / 2.,
                      y1 = (v1.y + 1.) * height / 2.;

            draw_line(img, x0, y0, x1, y1, WHITE);
        }
    }

    img.flipVertically();
    img.saveToFile("result.png");
}

Vec3f world2screen(const Vec3f v, int width, int height)
{
    return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}

void cmd_draw_filled_head()
{
    const float width = 800, height = 800;
    const Vec3f light(0, 0, -1);

    std::vector<float> zbuf(width * height, -std::numeric_limits<float>::max());

    sf::Image img;
    img.create(width, height, sf::Color::Black);

    Model model("obj/head.obj");

    for (size_t i = 0; i < model.nfaces(); ++i)
    {
        const std::vector<int> face = model.face(i);
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        for (size_t j = 0; j < 3; ++j)
        {
            const Vec3f v = model.vert(face[j]);
            screen_coords[j] = world2screen(v, width, height);
            world_coords[j] = v;
        }

        const float illumination = get_illumination(
            light,
            world_coords[2] - world_coords[0],
            world_coords[1] - world_coords[0]);
        const int color = 255 * illumination;

        if (illumination > 0.f)
        {
            draw_triangle(
                img,
                zbuf,
                screen_coords[0],
                screen_coords[1],
                screen_coords[2],
                sf::Color(color, color, color));
        }
    }

    img.flipVertically();
    img.saveToFile("result.png");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./tinyrenderer COMMAND" << std::endl;
        return 1;
    }

    const std::string command(argv[1]);
    if (command == "head")
    {
        cmd_draw_head();
    }
    else if (command == "filled-head")
    {
        cmd_draw_filled_head();
    }
    else
    {
        std::cerr << "Unknown command" << std::endl;
        return 1;
    }

    return 0;
}
