#include <cmath>

#include "algebra.hpp"

namespace nodesoup {

Point2D::operator Vector2D() const {
    return { x, y };
}

Point2D& Point2D::operator+=(const Vector2D& vector) {
    x += vector.dx;
    y += vector.dy;
    return *this;
}

Point2D& Point2D::operator-=(const Vector2D& vector) {
    x -= vector.dx;
    y -= vector.dy;
    return *this;
}

Vector2D::operator Point2D() const {
    return { dx, dy };
}

Vector2D& Vector2D::operator+=(const Vector2D& other) {
    dx += other.dx;
    dy += other.dy;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other) {
    dx -= other.dx;
    dy -= other.dy;
    return *this;
}

Vector2D& Vector2D::operator*=(double scalar) {
    dx *= scalar;
    dy *= scalar;
    return *this;
}

Vector2D& Vector2D::operator/=(double scalar) {
    dx /= scalar;
    dy /= scalar;
    return *this;
}

Point2D operator+(const Point2D& point, const Vector2D& vector) {
    return { point.x + vector.dx, point.y + vector.dy };
}

Point2D operator-(const Point2D& point, const Vector2D& vector) {
    return { point.x - vector.dx, point.y - vector.dy };
}

Vector2D operator-(const Point2D& lhs, const Point2D& rhs) {
    return { lhs.x - rhs.x, lhs.y - rhs.y };
}

Vector2D operator+(const Vector2D& lhs, const Vector2D& rhs) {
    return { lhs.dx + rhs.dx, lhs.dy + rhs.dy };
}

Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs) {
    return { lhs.dx - rhs.dx, lhs.dy - rhs.dy };
}

Vector2D operator*(const Vector2D& vector, double scalar) {
    return { vector.dx * scalar, vector.dy * scalar };
}

Vector2D operator*(double scalar, const Vector2D& vector) {
    return vector * scalar;
}

Vector2D operator/(const Vector2D& vector, double scalar) {
    return { vector.dx / scalar, vector.dy / scalar };
}
}
