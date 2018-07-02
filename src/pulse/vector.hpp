#pragma once

#include <cassert>
#include <cmath>
#include <ostream>
#include <type_traits>

namespace internal {

template <class T1, class T2>
struct SumType {
    using Type = decltype(std::declval<T1>() + std::declval<T2>());
};

template <class T1, class T2>
struct DiffType {
    using Type = decltype(std::declval<T1>() - std::declval<T2>());
};

template <class T1, class T2>
struct MulType {
    using Type = decltype(std::declval<T1>() * std::declval<T2>());
};

template <class T1, class T2>
struct DivType {
    using Type = decltype(std::declval<T1>() / std::declval<T2>());
};

} // namespace internal

template <class T1, class T2>
using SumType = typename internal::SumType<T1, T2>::Type;

template <class T1, class T2>
using DiffType = typename internal::DiffType<T1, T2>::Type;

template <class T1, class T2>
using MulType = typename internal::MulType<T1, T2>::Type;

template <class T1, class T2>
using DivType = typename internal::DivType<T1, T2>::Type;

template <class T>
struct Vector {
    static_assert(std::is_arithmetic<T>(), "Vector must hold arithmetic type");

    // Constructors

    constexpr Vector()
        : x{}, y{}
    { }

    constexpr Vector(T x, T y)
        : x(x), y(y)
    { }

    // Arithmetic operators

    Vector& operator+=(const Vector& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector& operator-=(const Vector& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector& operator*=(const T& scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector& operator/=(const T& scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    T length() const
    {
        // TODO: do not calculate sqrt right away
        return sqrt(x * x + y * y);
    }

    Vector normalized() const
    {
        auto len = length();
        assert(len > 0);
        return {x / len, y / len};
    }

    Vector normalized(Vector alternative) const
    {
        auto len = length();
        if (len == 0) {
            return alternative;
        }
        return {x / len, y / len};
    }

    Vector rotateCw90() const
    {
        return {y, -x};
    }

    Vector rotateCcw90() const
    {
        return {-y, x};
    }

    Vector projection(const Vector& axis) const
    {
        auto normalizedAxis = axis.normalized();
        auto coordinate = dot(*this, normalizedAxis);
        return normalizedAxis * coordinate;
    }

    Vector ort(const Vector& axis) const
    {
        return *this - projection(axis);
    }

    T x;
    T y;
};

// Arithmetic operators

template <class T>
Vector<T> operator-(Vector<T> vector)
{
    vector.x = -vector.x;
    vector.y = -vector.y;
    return vector;
}

template <class T>
Vector<T> operator+(Vector<T> left, const Vector<T>& right)
{
    left += right;
    return left;
}

template <class T>
Vector<T> operator-(Vector<T> left, const Vector<T>& right)
{
    left -= right;
    return left;
}

template <class T>
Vector<T> operator*(Vector<T> vector, T scalar)
{
    vector *= scalar;
    return vector;
}

template <class T>
Vector<T> operator*(T scalar, Vector<T> vector)
{
    vector *= scalar;
    return vector;
}

template <class T1, class T2>
Vector<DivType<T1, T2>> operator/(Vector<T1> vector, T2 scalar)
{
    return {vector.x / scalar, vector.y / scalar};
}

template <class Target, class Source>
Vector<Target> vectorCast(const Vector<Source>& source)
{
    return {static_cast<Target>(source.x), static_cast<Target>(source.y)};
}

template <class T>
T dot(const Vector<T>& left, const Vector<T>& right)
{
    return left.x * right.x + left.y * right.y;
}

template <class T>
T cross(const Vector<T>& left, const Vector<T>& right)
{
    return left.x * right.y - left.y * right.x;
}

template <class T>
std::ostream& operator<<(std::ostream& stream, const Vector<T>& vector)
{
    stream << "[" << vector.x << ", " << vector.y << "]";
    return stream;
}
