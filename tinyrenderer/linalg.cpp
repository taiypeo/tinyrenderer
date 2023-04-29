#include "linalg.hpp"

Matrix::Matrix(size_t rows, size_t cols)
{
    mat = std::vector<std::vector<float>>(rows, std::vector<float>(cols, 0.f));
}

std::vector<float> &Matrix::operator[](size_t idx)
{
    return mat[idx];
}

Matrix Matrix::operator*(const Matrix &other) const
{
    if (n_cols() != other.n_cols())
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