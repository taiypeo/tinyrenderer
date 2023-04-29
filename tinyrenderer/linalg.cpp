#include "linalg.hpp"

Matrix::Matrix(size_t rows, size_t cols)
{
    mat = std::vector<std::vector<float>>(rows, std::vector<float>(cols, 0.f));
}

Matrix::Matrix(const FloatVector &vec) : Matrix(VectorComponent::W + 1, 1)
{
    for (size_t i = VectorComponent::X; i <= VectorComponent::Z; ++i)
    {
        mat[i][0] = vec.at(i);
    }

    mat[VectorComponent::W][0] = 1.f;
}

Matrix Matrix::operator*(const Matrix &other) const
{
    if (n_cols() != other.n_rows())
    {
        throw std::runtime_error("Matrix dimensions are incompatible for multiplication");
    }

    Matrix result(n_rows(), other.n_cols());

    for (size_t i = 0; i < n_rows(); ++i)
    {
        for (size_t j = 0; j < other.n_cols(); ++j)
        {
            float val = 0.f;
            for (size_t k = 0; k < n_cols(); ++k)
            {
                val += mat[i][k] * other.mat[k][j];
            }

            result[i][j] = val;
        }
    }

    return result;
}

std::vector<float> &Matrix::operator[](size_t idx)
{
    return mat[idx];
}

const std::vector<float> &Matrix::at(size_t idx) const
{
    return mat[idx];
}

Matrix Matrix::T() const
{
    Matrix result(n_rows(), n_cols());

    for (size_t i = 0; i < n_rows(); ++i)
    {
        for (size_t j = 0; j < n_cols(); ++j)
        {
            result[i][j] = mat[j][i];
        }
    }

    return result;
}

size_t Matrix::n_rows() const
{
    return mat.size();
}

size_t Matrix::n_cols() const
{
    return mat[0].size();
}

Matrix Matrix::identity(size_t size)
{
    Matrix result(size, size);
    for (size_t i = 0; i < size; ++i)
    {
        result[i][i] = 1.f;
    }

    return result;
}

FloatVector Matrix::to_vector() const
{
    if (n_cols() != 1 || n_rows() != VectorComponent::W + 1)
    {
        throw std::runtime_error("Can only transform a 4x1 matrix into a vector");
    }

    FloatVector result;
    for (size_t i = VectorComponent::X; i <= VectorComponent::Z; ++i)
    {
        result[i] = mat[i][0] / mat[VectorComponent::W][0];
    }

    return result;
}