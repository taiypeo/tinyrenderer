#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../third-party/model.h"
#include "../third-party/geometry.h"

enum VectorComponent
{
    X,
    Y,
    Z
};

struct Triangle
{
    Vec3f p0, p1, p2;

    std::pair<Vec2i, Vec2i> bounding_box(int screen_width, int screen_height) const;
    float scale_barycentric(int component_idx, const Vec3f &barycentric) const;
    Vec3f &operator[](size_t idx);
};

class Renderer
{
private:
    sf::Image &screen, &texture;
    std::vector<std::vector<float>> zbuf;
    Model &model;
    const int screen_width, screen_height, texture_width, texture_height;
    const Vec3f light;

    void draw_triangle(
        const Triangle &triangle,
        const Triangle &texture_triangle,
        float illumination);
    void draw_line(int x0, int y0, int x1, int y1, const sf::Color &color);
    Vec3f world_to_screen(const Vec3f &vec);

public:
    Renderer(
        sf::Image &screen_,
        Model &model_,
        sf::Image &texture_,
        const Vec3f light_);

    void draw();
};

#endif
