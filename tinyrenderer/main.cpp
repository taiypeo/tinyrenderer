#include <algorithm>
#include <cmath>

#include <SFML/Graphics.hpp>

const sf::Color WHITE(255, 255, 255), RED(255, 0, 0);

void draw_line(sf::Image &img, int x0, int y0, int x1, int y1, const sf::Color &color)
{
    if (x1 < x0)
    {
        std::swap(x0, x1);
    }
    if (y1 < y0)
    {
        std::swap(y0, y1);
    }

    bool transpose = false;
    if (x1 - x0 < y1 - y0)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        transpose = true;
    }

    const int dy = y1 - y0, dx = x1 - x0;
    int two_error = 0, y = y0;
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

        two_error += 2 * dy;
        if (two_error > dx)
        {
            ++y;
            two_error -= 2 * dx;
        }
    }
}

int main()
{
    const float width = 100, height = 100;

    sf::Image img;
    img.create(width, height, sf::Color::Black);

    draw_line(img, 13, 20, 80, 40, WHITE);
    draw_line(img, 20, 13, 40, 80, RED);
    draw_line(img, 80, 40, 13, 20, RED);

    img.flipVertically();
    img.saveToFile("result.png");

    return 0;
}
