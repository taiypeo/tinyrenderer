#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <SFML/Graphics.hpp>

#include <vector>

#include "math/triangle.hpp"

struct Model
{
    std::vector<Triangle> faces, textures, normals;
    sf::Image texture, normal_map, diffuse_map, specular_map;

    Model(
        const std::string &model_filename,
        const std::string &texture_filename,
        const std::string &normal_map_filename,
        const std::string &specular_map_filename,
        const std::string &diffuse_map_filename);
};

#endif
