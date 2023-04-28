#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <SFML/Graphics.hpp>

#include <vector>

#include "../third-party/geometry.h"
#include "triangle.hpp"

struct Model
{
    std::vector<Triangle> faces, textures;
    sf::Image texture;

    Model(const std::string &model_filename, const std::string &texture_filename);
};

#endif
