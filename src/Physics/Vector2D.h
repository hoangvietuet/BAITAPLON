#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <cmath>
#include "Point.h"

class Vector2D {
public:
    float X, Y;

public:
    // Constructor mặc định
    Vector2D(float x = 0, float y = 0) : X(x), Y(y) {}

    // Constructor từ Point
    Vector2D(const Point& p) : X(p.X), Y(p.Y) {}

    // Phương thức chuẩn hóa vector
    Vector2D Normalized() const {
        float length = sqrt(X * X + Y * Y);
        if (length > 0) {
            return Vector2D(X / length, Y / length);
        }
        std::cerr << "Warning: Normalizing a zero-length vector" << std::endl;
        return Vector2D(0, 0);
    }

    // Tính độ dài của vector
    float Length() const {
        return sqrt(X * X + Y * Y);
    }

    // Toán tử chia cho float
    inline Vector2D operator/(const float scalar) const {
        if (scalar != 0)
            return Vector2D(X / scalar, Y / scalar);
        else {
            std::cerr << "Warning: Division by zero in Vector2D" << std::endl;
            return Vector2D(0, 0);
        }
    }

    // Tính khoảng cách giữa hai vector
    static float Distance(const Vector2D& a, const Vector2D& b) {
        float dx = a.X - b.X;
        float dy = a.Y - b.Y;
        return sqrt(dx * dx + dy * dy);
    }

    // Toán tử cộng
    inline Vector2D operator+(const Vector2D& v2) const {
        return Vector2D(X + v2.X, Y + v2.Y);
    }

    // Toán tử trừ
    inline Vector2D operator-(const Vector2D& v2) const {
        return Vector2D(X - v2.X, Y - v2.Y);
    }

    // Toán tử nhân với float
    inline Vector2D operator*(const float scalar) const {
        return Vector2D(X * scalar, Y * scalar);
    }

    // Phương thức ghi log thông tin vector
    void Log(std::string msg = "") const {
        std::cout << msg << " (X, Y) = (" << X << ", " << Y << ")" << std::endl;
    }
};

#endif // VECTOR2D_H
