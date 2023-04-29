#include "shader.hpp"

#include "math/triangle.hpp"

Shader::Shader(
    const Model &model,
    const Matrix &model_mat,
    const Matrix &view_mat,
    const Matrix &proj_mat,
    const Matrix &viewport_mat) : model(model),
                                  transformation_mat(viewport_mat * proj_mat * view_mat * model_mat) {}

SimpleShader::SimpleShader(
    const Model &model,
    const Matrix &model_mat,
    const Matrix &view_mat,
    const Matrix &proj_mat,
    const Matrix &viewport_mat,
    const FloatVector &light) : Shader(model, model_mat, view_mat, proj_mat, viewport_mat),
                                light(light),
                                texture_width(model.texture.getSize().x),
                                texture_height(model.texture.getSize().y) {}

FloatVector SimpleShader::vertex(size_t face_idx, size_t vertex_idx)
{
    const Triangle face = model.faces[face_idx];

    const FloatVector normal = (face.p2 - face.p0) ^ (face.p1 - face.p0);
    face_illumination = light * normal / (light.norm() * normal.norm());

    texture_triangle = model.textures[face_idx];

    return (transformation_mat * Matrix(face.at(vertex_idx))).to_vector();
}

bool SimpleShader::fragment(const FloatVector &barycentric, sf::Color &color)
{
    if (face_illumination <= 0.f)
    {
        color = sf::Color::Black;
        return false;
    }

    const float texture_x = texture_triangle.scale_barycentric(VectorComponent::X, barycentric) * texture_width,
                texture_y = texture_triangle.scale_barycentric(VectorComponent::Y, barycentric) * texture_height;

    color = model.texture.getPixel(texture_x, texture_y);
    color.r *= face_illumination;
    color.g *= face_illumination;
    color.b *= face_illumination;

    return false;
}