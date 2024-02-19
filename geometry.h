#ifndef SOFTRASTERIZATIONRENDERER_GEOMETRY_H
#define SOFTRASTERIZATIONRENDERER_GEOMETRY_H

#include <cmath>
#include <iostream>

template<class T>
struct Vec2 {
    T x, y;
    Vec2() : x(0), y(0) {}
    Vec2(T x, T y) : x(x), y(y) {}
    Vec2<T> operator+(const Vec2<T> V) const {
        return Vec2<T>(x + V.x, y + V.y);
    }
    Vec2<T> operator-(const Vec2<T> V) const {
        return Vec2<T>(x - V.x, y - V.y);
    }
    Vec2<T> operator*(float k) const {
        return Vec2<T>(x * k, y * k);
    }
};
typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

template<class T>
struct Vec3 {
    T x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    Vec3<T> operator^(const Vec3<T> &v) const {
        return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    Vec3<T> operator+(const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
    Vec3<T> operator-(const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
    Vec3<T> operator*(float f) const { return Vec3<T>(x * f, y * f, z * f); }
    T operator*(const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3<T> &normalize(T l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }
};
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
#endif //SOFTRASTERIZATIONRENDERER_GEOMETRY_H
