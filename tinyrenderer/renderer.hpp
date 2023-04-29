#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

#include "camera.hpp"
#include "triangle.hpp"
#include "model.hpp"
#include "linalg.hpp"

class Renderer
{
private:
    sf::Image &screen;
    std::vector<std::vector<float>> zbuf;
    Model &model;
    const Camera &camera;
    const FloatVector light;
    const int screen_width, screen_height, texture_width, texture_height;

    void draw_triangle(
        const Triangle &triangle,
        const Triangle &texture_triangle,
        float illumination);
    void draw_line(int x0, int y0, int x1, int y1, const sf::Color &color);

public:
    Renderer(
        sf::Image &screen,
        Model &model,
        const Camera &camera,
        const FloatVector light);

    void draw();
};

#endif
