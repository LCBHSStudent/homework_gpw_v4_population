//
// Created by tusimple on 2022/1/5.
//

#ifndef LIBGEO_COMMON_TYPES_H
#define LIBGEO_COMMON_TYPES_H

#include <array>
#include <vector>

template <typename T>
class Point {
public:
    T x = {};
    T y = {};

    explicit Point(const std::array<T, 2> &point) {
        this->x = point[0];
        this->y = point[1];
    }
    Point(double x_, double y_): x(x_), y(y_) {}
    Point() = default;
    bool operator==(const Point<T> &rhs) const {
        return rhs.x == this->x && rhs.y == this->y;
    }
    bool operator!=(const Point<T> &rhs) const { return !operator==(rhs); }
    Point<T>& operator=(const Point<T> &rhs) {
        this->x = rhs.x;
        this->y = rhs.y;

        return *this;
    }
};

using PointF = Point<double>;
using Segment = std::pair<PointF, PointF>;
class PointHash {
public:
    size_t operator()(Point<int> const &point) const {
        return std::hash<int>()(point.x) + std::hash<int>()(point.y);
    }
};

class Tile {
public:
    PointF leftBottom = {};
    PointF leftTop = {};
    PointF rightTop = {};
    PointF rightBottom = {};
};

inline std::pair<PointF, PointF> getBoundingBox(const Segment &segment) {
    return {
        {std::min(segment.first.x, segment.second.x), std::min(segment.first.y, segment.second.y)},
        {std::max(segment.first.x, segment.second.x), std::max(segment.first.y, segment.second.y)}
    };
}

constexpr double EPSILON = 1e-8;

#endif //LIBGEO_COMMON_TYPES_H
