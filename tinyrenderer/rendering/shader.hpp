#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <SFML/Graphics.hpp>

#include "math/linalg.hpp"
#include "rendering/model.hpp"

class Shader
{
protected:
    const Model &model;
    const Matrix transformation_mat;

public:
    Shader(
        const Model &model,
        const Matrix &model_mat,
        const Matrix &view_mat,
        const Matrix &proj_mat,
        const Matrix &viewport_mat);

    virtual FloatVector vertex(size_t face, size_t vert) = 0;
    virtual bool fragment(const FloatVector &barycentric, sf::Color &color) = 0;
};

class SimpleShader : public Shader
{
private:
    const FloatVector light;
    const int texture_width, texture_height;

    float face_illumination;
    Triangle texture_triangle;

public:
    SimpleShader(
        const Model &model,
        const Matrix &model_mat,
        const Matrix &view_mat,
        const Matrix &proj_mat,
        const Matrix &viewport_mat,
        const FloatVector &light);

    FloatVector vertex(size_t face, size_t vert);
    bool fragment(const FloatVector &barycentric, sf::Color &color);
};

#endif
