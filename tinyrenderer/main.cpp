#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <iostream>

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
    const Vec2i &p,
    const Vec2i &p0,
    const Vec2i &p1,
    const Vec2i &p2)
{
    const Vec2i p10 = p1 - p0,
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

void draw_triangle(sf::Image &img, Vec2i p0, Vec2i p1, Vec2i p2, const sf::Color &color)
{
    const int bbox_x0 = std::max(0, std::min({p0.x, p1.x, p2.x})),
              bbox_x1 = std::min(static_cast<int>(img.getSize().x) - 1, std::max({p0.x, p1.x, p2.x})),
              bbox_y0 = std::max(0, std::min({p0.y, p1.y, p2.y})),
              bbox_y1 = std::min(static_cast<int>(img.getSize().y) - 1, std::max({p0.y, p1.y, p2.y}));

    for (int x = bbox_x0; x <= bbox_x1; ++x)
    {
        for (int y = bbox_y0; y <= bbox_y1; ++y)
        {
            const Vec2i p(x, y);
            const Vec3f barycentric = barycentric_coords(p, p0, p1, p2);
            if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0)
            {
                continue;
            }

            img.setPixel(x, y, color);
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

void cmd_draw_filled_head()
{
    const float width = 800, height = 800;
    const Vec3f light(0, 0, -1);

    sf::Image img;
    img.create(width, height, sf::Color::Black);

    Model model("obj/head.obj");

    for (size_t i = 0; i < model.nfaces(); ++i)
    {
        const std::vector<int> face = model.face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (size_t j = 0; j < 3; ++j)
        {
            const Vec3f v = model.vert(face[j]);
            screen_coords[j] = Vec2i(
                (v.x + 1.) * width / 2.,
                (v.y + 1.) * height / 2.);
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
                screen_coords[0],
                screen_coords[1],
                screen_coords[2],
                sf::Color(color, color, color));
        }
    }

    img.flipVertically();
    img.saveToFile("result.png");
}

void cmd_draw_triangle()
{
    const float width = 200, height = 200;

    sf::Image img;
    img.create(width, height, sf::Color::Black);

    Vec2i p0[3] = {Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80)};
    Vec2i p1[3] = {Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180)};
    Vec2i p2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

    draw_triangle(img, p0[0], p0[1], p0[2], RED);
    draw_triangle(img, p1[0], p1[1], p1[2], WHITE);
    draw_triangle(img, p2[0], p2[1], p2[2], GREEN);

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
    else if (command == "triangle")
    {
        cmd_draw_triangle();
    }
    else
    {
        std::cerr << "Unknown command" << std::endl;
        return 1;
    }

    return 0;
}
