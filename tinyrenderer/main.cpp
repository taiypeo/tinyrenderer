#include <algorithm>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../third-party/model.h"
#include "../third-party/geometry.h"

const sf::Color WHITE(255, 255, 255), RED(255, 0, 0);

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

int main()
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

    return 0;
}
