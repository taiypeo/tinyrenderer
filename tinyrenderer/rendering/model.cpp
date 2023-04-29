#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "math/linalg.hpp"
#include "rendering/model.hpp"

void load_image(sf::Image &image, const std::string &filename)
{
    if (!image.loadFromFile(filename))
    {
        throw std::runtime_error("Failed to load image");
    }

    image.flipVertically();
}

void read_vector(std::istringstream &iss, std::vector<FloatVector> &vectors)
{
    FloatVector vec;
    iss >> vec.x >> vec.y >> vec.z;
    vectors.push_back(vec);
}

void fill_triangle_vector(
    const std::vector<IntVector> &indices,
    const std::vector<FloatVector> &coordinates,
    std::vector<Triangle> &target)
{
    for (const auto &idx : indices)
    {
        target.emplace_back(coordinates[idx.x - 1], coordinates[idx.y - 1], coordinates[idx.z - 1]);
    }
}

void load_wavefront(
    const std::string &model_filename,
    std::vector<Triangle> &faces,
    std::vector<Triangle> &textures,
    std::vector<Triangle> &normals)
{
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
        std::string line_type;
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
            char char_discard;
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

    fill_triangle_vector(face_indices, vertices, faces);
    fill_triangle_vector(texture_indices, texture_coordinates, textures);
    fill_triangle_vector(normal_indices, normal_vectors, normals);
}

Model::Model(
    const std::string &model_filename,
    const std::string &normal_map_filename,
    const std::string &specular_map_filename,
    const std::string &diffuse_map_filename)
{
    load_image(normal_map, normal_map_filename);
    load_image(specular_map, specular_map_filename);
    load_image(diffuse_map, diffuse_map_filename);

    load_wavefront(
        model_filename,
        faces,
        textures,
        normals);
}

FloatVector Model::get_normal(size_t pixel_x, size_t pixel_y) const
{
    const sf::Color color = normal_map.getPixel(pixel_x, pixel_y);
    return FloatVector(
        color.r / 255.f * 2.f - 1.f,
        color.g / 255.f * 2.f - 1.f,
        color.b / 255.f * 2.f - 1.f);
}