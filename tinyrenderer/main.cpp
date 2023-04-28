#include <SFML/Graphics.hpp>

#include "model.hpp"
#include "renderer.hpp"

int main()
{
    sf::Image screen;
    screen.create(800, 800, sf::Color::Black);

    Model model("model/head.obj", "model/texture.png");

    const Vec3f light(0, 0, -1);

    Renderer renderer(screen, model, light);
    renderer.draw();

    screen.saveToFile("result.png");

    return 0;
}
