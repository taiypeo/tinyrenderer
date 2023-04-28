#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "model.hpp"

Model::Model(const std::string &filename)
{
    std::ifstream file(filename);
    if (file.fail())
    {
        throw std::runtime_error("Failed to read the model");
    }

    std::vector<Vec3f> vertices, texture_coordinates;
    std::vector<Vec3i> face_indices, texture_indices;

    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);
        std::istringstream iss(line);

        std::string line_type, str_discard;
        char char_discard;
        int int_discard;

        iss >> line_type;
        if (line_type == "v")
        {
            Vec3f vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (line_type == "vt")
        {
            Vec3f texture_coords;
            iss >> texture_coords.x >> texture_coords.y >> texture_coords.z;
            texture_coordinates.push_back(texture_coords);
        }
        else if (line_type == "f")
        {
            Vec3i face, texture;
            iss >> face.x >> char_discard >> texture.x >> char_discard >> int_discard;
            iss >> face.y >> char_discard >> texture.y >> char_discard >> int_discard;
            iss >> face.z >> char_discard >> texture.z >> char_discard >> int_discard;

            face_indices.push_back(face);
            texture_indices.push_back(texture);
        }
    }

    for (const auto &face : face_indices)
    {
        faces.emplace_back(vertices[face.x - 1], vertices[face.y - 1], vertices[face.z - 1]);
    }

    for (const auto &texture_idx : texture_indices)
    {
        textures.emplace_back(
            texture_coordinates[texture_idx.x - 1],
            texture_coordinates[texture_idx.y - 1],
            texture_coordinates[texture_idx.z - 1]);
    }
}
