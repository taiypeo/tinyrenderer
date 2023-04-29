#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "rendering/model.hpp"
#include "math/linalg.hpp"

void read_vector(std::istringstream &iss, std::vector<FloatVector> &vectors)
{
    FloatVector vec;
    iss >> vec.x >> vec.y >> vec.z;
    vectors.push_back(vec);
}

Model::Model(
    const std::string &model_filename,
    const std::string &texture_filename)
{
    if (!texture.loadFromFile(texture_filename))
    {
        throw std::runtime_error("Failed to load the texture");
    }

    texture.flipVertically();

    std::ifstream file(model_filename);
    if (file.fail())
    {
        throw std::runtime_error("Failed to load the model");
    }

    std::vector<FloatVector> vertices, texture_coordinates, normal_vectors;
    std::vector<IntVector> face_indices, texture_indices, normal_indices;

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
            read_vector(iss, vertices);
        }
        else if (line_type == "vt")
        {
            read_vector(iss, texture_coordinates);
        }
        else if (line_type == "vn")
        {
            read_vector(iss, normal_vectors);
        }
        else if (line_type == "f")
        {
            IntVector face, texture, normal;
            for (size_t component = VectorComponent::X; component <= VectorComponent::Z; ++component)
            {
                iss >> face[component] >> char_discard >> texture[component] >> char_discard >> normal[component];
            }

            face_indices.push_back(face);
            texture_indices.push_back(texture);
            normal_indices.push_back(normal);
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

    for (const auto &normal_idx : normal_indices)
    {
        normals.emplace_back(
            normal_vectors[normal_idx.x - 1],
            normal_vectors[normal_idx.y - 1],
            normal_vectors[normal_idx.z - 1]);
    }
}
