#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <vector>

#include <SFML/Graphics.hpp>

#include "math/linalg.hpp"
#include "math/triangle.hpp"
#include "rendering/shader.hpp"

void draw_triangle(
    sf::Image &screen,
    const Triangle &triangle,
    std::vector<std::vector<float>> &zbuf,
    Shader &shader);

void draw_line(
    sf::Image &screen,
    int x0,
    int y0,
    int x1,
    int y1,
    const sf::Color &color);

#endif
