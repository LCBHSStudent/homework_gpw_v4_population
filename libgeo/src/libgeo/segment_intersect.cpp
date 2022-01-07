//
// Created by tusimple on 2022/1/5.
//

#include "libgeo/segment_intersect.h"

#include <cmath>

bool isBoundingBoxesIntersect(const Segment &a, const Segment &b) {
    return (
        a.first.x <= b.second.x &&
        a.second.x >= b.first.x &&
        a.first.y <= b.second.y &&
        a.second.y >= b.first.y
    );
}

bool isPointOnSegment(const Segment &a, const PointF &b) {
    Segment tempA({{}, {a.second.x - a.first.x, a.second.y - a.first.y}});
    PointF tempB(b.x - a.first.x, b.y - a.first.y);

    return fabs(crsProduct(tempA.second, tempB)) < EPSILON;
}

bool isPointRightOfSegment(const Segment &a, const PointF &b) {
    Segment tempA({{}, {a.second.x - a.first.x, a.second.y - a.first.y}});
    PointF tempB(b.x - a.first.x, b.y - a.first.y);

    return crsProduct(tempA.second, tempB) < 0;
}

bool isSegmentTouchesOrCrossOther(const Segment &a, const Segment &b) {
    return (
        isPointOnSegment(a, b.first) ||
        isPointOnSegment(a, b.second) || (
            isPointRightOfSegment(a, b.first) ^ isPointRightOfSegment(a, b.second)
        )
    );
}

bool isSegmentIntersect(const Segment &a, const Segment &b) {
    auto&& boxA = getBoundingBox(a);
    auto&& boxB = getBoundingBox(b);

    return (
        isBoundingBoxesIntersect(boxA, boxB) &&
        isSegmentTouchesOrCrossOther(a, b) &&
        isSegmentTouchesOrCrossOther(b, a)
    );
}
