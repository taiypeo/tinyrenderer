#include <SFML/Graphics.hpp>

#include "model.hpp"
#include "renderer.hpp"
#include "linalg.hpp"

int main()
{
    sf::Image screen;
    screen.create(800, 800, sf::Color::Black);

    Model model("model/head.obj", "model/texture.png");

    const FloatVector light(0.f, 0.f, -1.f);

    Renderer renderer(screen, model, light);
    renderer.draw();

    screen.saveToFile("result.png");

    return 0;
}
