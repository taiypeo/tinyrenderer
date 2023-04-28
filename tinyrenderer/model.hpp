#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <vector>

#include "../third-party/geometry.h"
#include "triangle.hpp"

struct Model
{
    std::vector<Triangle> faces, textures;

    Model(const std::string &filename);
};

#endif
