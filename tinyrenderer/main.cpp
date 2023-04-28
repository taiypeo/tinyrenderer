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

void sort_by_y(Vec2i &p0, Vec2i &p1, Vec2i &p2)
{
    if (p0.y > p1.y)
    {
        std::swap(p0, p1);
    }
    if (p1.y > p2.y)
    {
        std::swap(p1, p2);
    }
    if (p0.y > p1.y)
    {
        std::swap(p0, p1);
    }

    if (!(p0.y <= p1.y && p1.y <= p2.y))
    {
        throw std::runtime_error("Points are not sorted");
    }
}

void fill_top_triangle(sf::Image &img, const Vec2i &top, const Vec2i &left, const Vec2i &right, const sf::Color &color)
{
    for (int y = top.y + 1; y <= left.y; ++y)
    {
        int left_x = std::round(static_cast<float>(left.x - top.x) / (left.y - top.y) * (y - top.y) + top.x);
        int right_x = std::round(static_cast<float>(right.x - top.x) / (right.y - top.y) * (y - top.y) + top.x);
        draw_line(img, left_x, y, right_x, y, color);
    }
}

void fill_bottom_triangle(
    sf::Image &img,
    const Vec2i &bottom,
    const Vec2i &left,
    const Vec2i &right,
    const sf::Color &color)
{
    for (int y = bottom.y - 1; y >= left.y; --y)
    {
        int left_x = std::round(static_cast<float>(left.x - bottom.x) / (left.y - bottom.y) * (y - bottom.y) + bottom.x);
        int right_x = std::round(static_cast<float>(right.x - bottom.x) / (right.y - bottom.y) * (y - bottom.y) + bottom.x);
        draw_line(img, left_x, y, right_x, y, color);
    }
}

void draw_triangle(sf::Image &img, Vec2i p0, Vec2i p1, Vec2i p2, const sf::Color &color)
{
    draw_line(img, p0.x, p0.y, p1.x, p1.y, color);
    draw_line(img, p1.x, p1.y, p2.x, p2.y, color);
    draw_line(img, p2.x, p2.y, p0.x, p0.y, color);

    if (p0.y == p1.y && p1.y == p2.y || p0.x == p1.x && p1.x == p2.x)
    {
        return;
    }

    sort_by_y(p0, p1, p2);

    if (p0.y == p1.y)
    {
        if (p0.x > p1.x)
        {
            std::swap(p0, p1);
        }

        fill_bottom_triangle(img, p2, p0, p1, color);
        return;
    }
    if (p1.y == p2.y)
    {
        if (p1.x > p2.x)
        {
            std::swap(p1, p2);
        }

        fill_top_triangle(img, p0, p1, p2, color);
        return;
    }

    Vec2i division(static_cast<float>(p2.x - p0.x) / (p2.y - p0.y) * (p1.y - p0.y) + p0.x, p1.y);
    if (division.x > p1.x)
    {
        std::swap(division, p1);
    }

    draw_line(img, p1.x, p1.y, division.x, division.y, color);
    fill_top_triangle(img, p0, division, p1, color);
    fill_bottom_triangle(img, p2, division, p1, color);
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

    sf::Image img;
    img.create(width, height, sf::Color::Black);

    Model model("obj/head.obj");

    for (size_t i = 0; i < model.nfaces(); ++i)
    {
        const std::vector<int> face = model.face(i);
        Vec2i screen_coords[3];
        for (size_t j = 0; j < 3; ++j)
        {
            const Vec3f world_coords = model.vert(face[j]);
            screen_coords[j] = Vec2i(
                (world_coords.x + 1.) * width / 2.,
                (world_coords.y + 1.) * height / 2.);
        }

        draw_triangle(
            img,
            screen_coords[0],
            screen_coords[1],
            screen_coords[2],
            sf::Color(rand() % 255, rand() % 255, rand() % 255));
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
