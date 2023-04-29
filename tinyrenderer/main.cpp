#include <SFML/Graphics.hpp>

#include "rendering/camera.hpp"
#include "rendering/model.hpp"
#include "rendering/renderer.hpp"
#include "math/linalg.hpp"

int main()
{
    sf::Image screen;
    screen.create(800, 800, sf::Color::Black);

    Model model("model/head.obj", "model/texture.png");

    Camera camera(
        FloatVector(1.f, 1.f, 3.f),
        FloatVector(0.f, 0.f, 0.f),
        FloatVector(0.f, 1.f, 0.f));

    Renderer renderer(screen, model, camera, FloatVector(1.f, -1.f, -1.f));
    renderer.draw();

    screen.saveToFile("result.png");

    return 0;
}
