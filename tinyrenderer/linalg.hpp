#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <cmath>
#include <stdexcept>
#include <vector>

enum VectorComponent
{
    X,
    Y,
    Z,
    W
};

template <typename T>
struct Vector
{
    T x, y, z;

    Vector() : x(0), y(0), z(0) {}
    Vector(T x, T y) : x(x), y(y), z(0) {}
    Vector(T x, T y, T z) : x(x), y(y), z(z) {}

    Vector<T> operator+(const Vector<T> &other) const
    {
        return Vector<T>(x + other.x, y + other.y, z + other.z);
    }

    Vector<T> operator-(const Vector<T> &other) const
    {
        return Vector<T>(x - other.x, y - other.y, z - other.z);
    }

    Vector<T> operator*(T constant) const
    {
        return Vector<T>(x * constant, y * constant, z * constant);
    }

    T operator*(const Vector<T> &other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector<T> operator^(const Vector<T> &other) const
    {
        return Vector<T>(
            y * other.z - other.y * z,
            -x * other.z + other.x * z,
            x * other.y - other.x * y);
    }

    T norm() const
    {
        const Vector<T> &ref = *this;
        return std::sqrt(ref * ref);
    }

    T &operator[](size_t idx)
    {
        switch (idx)
        {
        case VectorComponent::X:
            return x;
        case VectorComponent::Y:
            return y;
        case VectorComponent::Z:
            return z;
        default:
            throw std::runtime_error("Invalid vector component index");
        }
    }

    const T at(size_t idx) const
    {
        switch (idx)
        {
        case VectorComponent::X:
            return x;
        case VectorComponent::Y:
            return y;
        case VectorComponent::Z:
            return z;
        default:
            throw std::runtime_error("Invalid vector component index");
        }
    }
};

using IntVector = Vector<int>;
using FloatVector = Vector<float>;

class Matrix
{
private:
    std::vector<std::vector<float>> mat;

public:
    Matrix(size_t rows, size_t cols);
    Matrix(const FloatVector &vec);

    Matrix operator*(const Matrix &other) const;
    std::vector<float> &operator[](size_t idx);
    const std::vector<float> &at(size_t idx) const;
    Matrix T() const;
    size_t n_rows() const;
    size_t n_cols() const;
    FloatVector to_vector() const;

    static Matrix identity(size_t size);
};

#endif
