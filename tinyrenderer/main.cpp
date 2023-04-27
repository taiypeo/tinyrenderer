#include <algorithm>
#include <cmath>

#include <SFML/Graphics.hpp>

const sf::Color WHITE(255, 255, 255), RED(255, 0, 0);

void draw_line(sf::Image &img, int x0, int y0, int x1, int y1, const sf::Color color)
{
    for (float t = 0.; t < 1.; t += .01)
    {
        int x = x0 * (1. - t) + x1 * t;
        int y = y0 * (1. - t) + y1 * t;
        img.setPixel(x, y, color);
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
