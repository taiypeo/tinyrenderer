#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <SFML/Graphics.hpp>

#include "math/linalg.hpp"
#include "rendering/model.hpp"

class Shader
{
protected:
    const Model &model;
    const Matrix model_mat, view_mat, proj_mat, viewport_mat, transformation_mat;

public:
    Shader(
        const Model &model,
        const Matrix &model_mat,
        const Matrix &view_mat,
        const Matrix &proj_mat,
        const Matrix &viewport_mat);

    virtual FloatVector vertex(size_t face_idx, size_t vertex_idx) = 0;
    virtual bool fragment(const FloatVector &barycentric, sf::Color &color) = 0;
};

class SimpleShader : public Shader
{
private:
    float face_illumination;

protected:
    const FloatVector light;
    const int texture_width, texture_height;

    Triangle texture_triangle;

public:
    SimpleShader(
        const Model &model,
        const Matrix &model_mat,
        const Matrix &view_mat,
        const Matrix &proj_mat,
        const Matrix &viewport_mat,
        const FloatVector &light);

    virtual FloatVector vertex(size_t face_idx, size_t vertex_idx);
    virtual bool fragment(const FloatVector &barycentric, sf::Color &color);
};

class GouraudShader : public SimpleShader
{
private:
    FloatVector varying_illumination;

public:
    using SimpleShader::SimpleShader;

    FloatVector vertex(size_t face_idx, size_t vertex_idx);
    bool fragment(const FloatVector &barycentric, sf::Color &color);
};

class NormalShader : public SimpleShader
{
private:
    Matrix before_viewport, before_viewport_tinv;
    const float ambient_const, diffuse_const, specular_const;

public:
    NormalShader(
        const Model &model,
        const Matrix &model_mat,
        const Matrix &view_mat,
        const Matrix &proj_mat,
        const Matrix &viewport_mat,
        const FloatVector &light,
        float ambient_const = 3.f,
        float diffuse_const = 1.2f,
        float specular_const = .6f);

    FloatVector vertex(size_t face_idx, size_t vertex_idx);
    bool fragment(const FloatVector &barycentric, sf::Color &color);
};

#endif
