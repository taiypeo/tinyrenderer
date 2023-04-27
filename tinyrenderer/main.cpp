#include <SFML/Graphics.hpp>

void display_image(sf::RenderWindow &window, const sf::Image &img)
{
    sf::Texture texture;
    texture.loadFromImage(img);

    sf::Sprite sprite;
    sprite.setTexture(texture, true);

    window.draw(sprite);
}

void draw_line(sf::Image &img, int x0, int y0, int x1, int y1, const sf::Color color)
{
    for (float t = 0.; t < 1.; t += 0.01)
    {
        float x = x0 + t * (x1 - x0), y = y0 + t * (y1 - y0);
        img.setPixel(x, y, color);
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "TinyRenderer");
    const auto size = window.getSize();
    const double width = size.x, height = size.y;

    sf::Image img;

    while (window.isOpen())
    {
        sf::Event event;
        if (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();

        img.create(width, height, sf::Color::Black);
        draw_line(img, 13, 20, 80, 40, sf::Color(255, 255, 255));
        display_image(window, img);

        window.display();
    }

    return 0;
}
