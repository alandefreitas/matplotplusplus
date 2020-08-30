#pragma once
#include "nodesoup.hpp"

namespace nodesoup {

Point2D operator+(const Point2D& point, const Vector2D& vector);
Point2D operator-(const Point2D& point, const Vector2D& vector);
Vector2D operator-(const Point2D& lhs, const Point2D& rhs);
Vector2D operator+(const Vector2D lhs, const Vector2D& rhs);
Vector2D operator-(const Vector2D& l, const Vector2D& rhs);
Vector2D operator*(const Vector2D& vector, double scalar);
Vector2D operator*(double scalar, const Vector2D& vector);
Vector2D operator/(const Vector2D& vector, double scalar);
};
