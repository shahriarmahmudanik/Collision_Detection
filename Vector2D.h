/*
 * Vector2D.h
 * 2D Vector class for position and velocity
 */

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

class Vector2D {
public:
    float x, y;

    // Default constructor
    Vector2D() : x(0), y(0) {}

    // Constructor with values
    Vector2D(float x, float y) : x(x), y(y) {}

    // Add two vectors
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    // Subtract two vectors
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    // Multiply by scalar
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    // Add and assign
    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Subtract and assign
    Vector2D& operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Reverse direction
    void reverseX() { x = -x; }
    void reverseY() { y = -y; }

    // Calculate magnitude (length) of vector
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalize vector (make it unit length)
    void normalize() {
        float mag = magnitude();
        if (mag > 0) {
            x /= mag;
            y /= mag;
        }
    }

    // Dot product with another vector
    float dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }
};

#endif // VECTOR2D_H
