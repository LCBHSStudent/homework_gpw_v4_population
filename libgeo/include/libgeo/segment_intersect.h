//
// Created by tusimple on 2022/1/5.
//

#ifndef LIBGEO_SEGMENT_INTERSECT_H
#define LIBGEO_SEGMENT_INTERSECT_H

#include "common_types.h"

/**
 * @brief Check if bounding boxes do intersect. If one bounding box touches the other, they do intersect.
 * @param a segment
 * @param b segment
 * @return true: intersect; false: otherwise
 */
__attribute__((visibility("default"))) bool isBoundingBoxesIntersect(const Segment &a, const Segment &b);

/**
 * @brief Get cross product of point a and b
 * @param a point
 * @param b point
 * @return result
 */
inline double crsProduct(const PointF &a, const PointF &b) {
    return a.x * b.y - b.x * a.y;
}

/**
 * @brief Checks if a Point is on a segment(line)
 * @param a segment
 * @param b point
 * @return true: if point is on the segment; false: otherwise
 */
__attribute__((visibility("default"))) bool isPointOnSegment(const Segment &a, const PointF &b);

/**
 * @brief Check if a Point is at the right of the segment(line)
 * @param a segment
 * @param b point
 * @return true: if point is at the right of the segment; false: otherwise
 */
__attribute__((visibility("default"))) bool isPointRightOfSegment(const Segment &a, const PointF &b);

/**
 * @brief Check if segment first touches or crosses the other segment
 * @param a segment
 * @param b other segment
 */
__attribute__((visibility("default"))) bool isSegmentTouchesOrCrossOther(const Segment &a, const Segment &b);

/**
 * @brief Check if segments intersect
 * @param a segment
 * @param b segment
 */
__attribute__((visibility("default"))) bool isSegmentIntersect(const Segment &a, const Segment &b);

#endif //LIBGEO_SEGMENT_INTERSECT_H
