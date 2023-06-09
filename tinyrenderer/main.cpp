#include <limits>
#include <vector>

#include <SFML/Graphics.hpp>

#include "math/linalg.hpp"
#include "rendering/model.hpp"
#include "rendering/draw.hpp"
#include "rendering/shader.hpp"

int main()
{
    const int screen_width = 1600, screen_height = 1600;

    sf::Image screen;
    screen.create(screen_width, screen_height, sf::Color::Black);

    Model model(
        "model/model.obj",
        "model/normal_map.png",
        "model/specular_map.png",
        "model/diffuse_map.png");

    const float ambient_const = 3.f, diffusion_const = 1.2f, specular_const = .6f;

    const FloatVector eye(1.f, 1.f, 3.f),
        center(0.f, 0.f, 0.f),
        up(0.f, 1.f, 0.f),
        light(0.f, 0.f, 1.f);

    const Matrix model_mat = Matrix::identity(4),
                 view_mat = Matrix::look_at(eye, center, up),
                 proj_mat = Matrix::projection((center - eye).norm()),
                 viewport_mat = Matrix::viewport(
                     screen_width / 8,
                     screen_height / 8,
                     screen_width * 3 / 4,
                     screen_height * 3 / 4);

    NormalShader shader(
        model,
        model_mat,
        view_mat,
        proj_mat,
        viewport_mat,
        light,
        ambient_const,
        diffusion_const,
        specular_const);

    std::vector<std::vector<float>> zbuf(
        screen_width,
        std::vector<float>(screen_height, -std::numeric_limits<float>::max()));

    for (size_t face_idx = 0; face_idx < model.faces.size(); ++face_idx)
    {
        Triangle screen_coords;
        for (size_t vertex_idx = 0; vertex_idx < 3; ++vertex_idx)
        {
            screen_coords[vertex_idx] = shader.vertex(face_idx, vertex_idx);
        }

        draw_triangle(screen, screen_coords, zbuf, shader);
    }

    screen.flipVertically();
    screen.saveToFile("result.png");

    return 0;
}
