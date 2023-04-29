#include "math/linalg.hpp"

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

Matrix Matrix::identity(size_t size)
{
    Matrix result(size, size);
    for (size_t i = 0; i < size; ++i)
    {
        result[i][i] = 1.f;
    }

    return result;
}

Matrix Matrix::look_at(const FloatVector &eye, const FloatVector &center, const FloatVector up)
{
    const FloatVector z = (eye - center).normalize(),
                      x = (up ^ z).normalize(),
                      y = (z ^ x).normalize();

    // Change of basis matrix M is orthogonal, therefore M^{-1} = M^T
    Matrix Minv = Matrix::identity(4), translate = Matrix::identity(4);
    for (size_t i = 0; i < 3; ++i)
    {
        Minv[0][i] = x.at(i);
        Minv[1][i] = y.at(i);
        Minv[2][i] = z.at(i);

        translate[i][3] = -eye.at(i);
    }

    return Minv * translate;
}

Matrix Matrix::viewport(int corner_x, int corner_y, int width, int height)
{
    Matrix vp = Matrix::identity(4);

    // Scaling
    vp[0][0] = width / 2.f;
    vp[1][1] = height / 2.f;
    vp[2][2] = 255 / 2.f;

    // Translation
    vp[0][3] = corner_x + width / 2.f;
    vp[1][3] = corner_y + height / 2.f;
    vp[2][3] = 255 / 2.f;

    return vp;
}

Matrix Matrix::projection(float camera_z)
{
    Matrix proj = Matrix::identity(4);
    proj[3][2] = -1.f / camera_z;
    return proj;
}