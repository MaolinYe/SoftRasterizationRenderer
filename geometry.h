#ifndef SOFTRASTERIZATIONRENDERER_GEOMETRY_H
#define SOFTRASTERIZATIONRENDERER_GEOMETRY_H

#include <cmath>
#include <iostream>

template<class T>
struct Vector2 {
    T x, y;
    Vector2() : x(0), y(0) {}
    Vector2(T x, T y) : x(x), y(y) {}
    Vector2<T> operator+(const Vector2<T> V) const {
        return Vector2<T>(x + V.x, y + V.y);
    }
    Vector2<T> operator-(const Vector2<T> V) const {
        return Vector2<T>(x - V.x, y - V.y);
    }
    Vector2<T> operator*(float k) const {
        return Vector2<T>(x * k, y * k);
    }
};
typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;

template<class T>
struct Vector3 {
    T x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    Vector3<T> operator^(const Vector3<T> &v) const {
        return Vector3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    Vector3<T> operator+(const Vector3<T> &v) const { return Vector3<T>(x + v.x, y + v.y, z + v.z); }
    Vector3<T> operator-(const Vector3<T> &v) const { return Vector3<T>(x - v.x, y - v.y, z - v.z); }
    Vector3<T> operator*(float f) const { return Vector3<T>(x * f, y * f, z * f); }
    T operator*(const Vector3<T> &v) const { return x * v.x + y * v.y + z * v.z; }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vector3<T> &normalize(T l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }
};
typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;
#endif //SOFTRASTERIZATIONRENDERER_GEOMETRY_H
