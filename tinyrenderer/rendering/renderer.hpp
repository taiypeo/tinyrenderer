#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

#include "math/linalg.hpp"
#include "math/triangle.hpp"
#include "rendering/model.hpp"
#include "rendering/shader.hpp"

class Renderer
{
private:
    sf::Image &screen;
    std::vector<std::vector<float>> zbuf;
    Model &model;
    Shader &shader;
    const FloatVector light;
    const int screen_width, screen_height, texture_width, texture_height;

    void draw_triangle(const Triangle &triangle);
    void draw_line(int x0, int y0, int x1, int y1, const sf::Color &color);

public:
    Renderer(
        sf::Image &screen,
        Model &model,
        Shader &shader,
        const FloatVector light);

    void draw();
};

#endif
