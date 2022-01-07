//
// Created by tusimple on 2022/1/5.
//

#include "libgeo/cal_grid_row.h"
#include "libgeo/segment_intersect.h"

#include <unordered_map>
#include <map>

using namespace std;
using TileStateMap = unordered_map<Point<int>, bool, PointHash>;

constexpr int INT_NONE = 0;
constexpr int INT_LEFT = 1;
constexpr int INT_TOP = 2;
constexpr int INT_RIGHT = 3;
constexpr int INT_BOTTOM = 4;

constexpr bool ODD = false;
constexpr bool EVEN = true;

constexpr int gridSize = 10800;


int testSegmentIntersectTile(
    const Segment   &baseSegment,
    const Tile      &tile,
    int             expected
) {
    // avoid death-loop
    int start = INT_LEFT;
    int end = INT_BOTTOM;
    if (expected != INT_NONE) {
        start = expected + 1;
        end = start + 4;
    }

    const auto getVertex = [&tile](PointF &p1, PointF &p2, int index) {
        switch (index) {
        case INT_LEFT:
            p1 = tile.leftBottom;
            p2 = tile.leftTop;
            break;
        case INT_TOP:
            p1 = tile.leftTop;
            p2 = tile.rightTop;
            break;
        case INT_RIGHT:
            p1 = tile.rightTop;
            p2 = tile.rightBottom;
            break;
        case INT_BOTTOM:
            p1 = tile.rightBottom;
            p2 = tile.leftBottom;
            break;
        default:
            break;
        }
    };

    for (auto i = start; i <= end; i++) {
        int edgeType = i % 5;
        if (edgeType == INT_NONE) {
            continue;
        }
        PointF p1;
        PointF p2;
        getVertex(p1, p2, edgeType);

        const Segment edge(p1, p2);
        if (isSegmentIntersect(baseSegment, edge)) {
            return edgeType;
        }
    }

    return INT_NONE;
}

void updateTileState(TileStateMap &M, const Point<int> &currentTile, int direction) {
    if (M.find(currentTile) != M.end()) {
        if (direction == INT_LEFT) {
            M[currentTile] = !M[currentTile];
        }
    } else {
        if (direction == INT_TOP || direction == INT_BOTTOM || direction == INT_RIGHT) {
            M.insert({currentTile, EVEN});
        } else if (direction == INT_LEFT) {
            M.insert({currentTile, ODD});
        }
    }
}


TileStateMap FindBorderTiles(const vector<Segment> &segments, const double grid) {
    TileStateMap M;

    for (const auto& segment : segments) {
        Point<int> startTile(static_cast<int>(segment.first.x / grid), static_cast<int>(segment.first.y / grid));
        Point<int> endTile(static_cast<int>(segment.second.x / grid), static_cast<int>(segment.second.y / grid));

        Point<int> currentTile = startTile;
        int expected = INT_NONE;

        while(currentTile != endTile) {
            PointF leftBottom(currentTile.x * grid, (currentTile.y + 1) * grid);
            PointF leftTop(currentTile.x * grid, currentTile.y * grid);
            PointF rightBottom((currentTile.x + 1) * grid, (currentTile.y + 1) * grid);
            PointF rightTop((currentTile.x + 1) * grid, currentTile.y * grid);

            Tile tile = {leftBottom, leftTop, rightTop, rightBottom};

            auto direction = testSegmentIntersectTile(segment, tile, expected);
            if (direction != INT_NONE) {
                updateTileState(M, currentTile, direction);

                switch (direction) {
                case INT_LEFT:
                    currentTile.x -= 1;
                    expected = INT_RIGHT;
                    break;
                case INT_TOP:
                    currentTile.y -= 1;
                    expected = INT_BOTTOM;
                    break;
                case INT_RIGHT:
                    currentTile.x += 1;
                    expected = INT_LEFT;
                    break;
                case INT_BOTTOM:
                    currentTile.y += 1;
                    expected = INT_TOP;
                    break;
                default:
                    break;
                }
            }
        }
    }
    return M;
}

[[maybe_unused]] auto makeCellRanges(const TileStateMap &M, bool single_ring = false) {
    map<int, vector<pair<Point<int>, bool>>, std::less<>> tp;
    for (auto& m : M) {
        if (tp.find(m.first.x) != tp.end()) {
            tp[m.first.x].emplace_back(m);
        } else {
            tp[m.first.x] = {m};
        }
    }

    vector<pair<int, vector<array<int, 2>>>> result = {};
    for (auto& col : tp) {
        if (col.second.empty()) {
            continue;
        }
        std::sort(col.second.begin(), col.second.end(),[](const pair<Point<int>, bool> &i1, const pair<Point<int>, bool> &i2) {
            return (i1.first.y <= i2.first.y);
        });

        vector<array<int, 2>> ranges = {};
        for (size_t i = 0, size = col.second.size(); i < size; ++i) {
            const auto& tile = col.second[i];
            auto lowerBound = tile.first.y;

            i += 1;
            std::pair<Point<int>, bool> tempTile = tile;
            if (single_ring) {
                while ((i < size) && (tempTile.first.y == col.second[i].first.y - 1)) { // NOLINT
                    tempTile = col.second[i];
                    i += 1;
                }
                if (i < size) {
                    tempTile = col.second[i];
                }
            } else {
                auto state = tile.second;
                while ((i < size) && ((tempTile.first.y == col.second[i].first.y - 1) || state == ODD)) { // NOLINT
                    const auto& t = col.second[i];
                    if (t.second == ODD) { // NOLINT
                        state = !state;
                    }
                    tempTile = t;
                    i += 1;
                }
            }

            auto upperBound = tempTile.first.y;
            ranges.emplace_back(array<int, 2>{lowerBound, upperBound});
        }

        result.emplace_back(col.first, ranges);
    }

    return result;
}

auto makeCellRangesHorizontal(const TileStateMap &M) {
    map<int, vector<pair<Point<int>, bool>>, std::less<>> tp;
    for (auto& m : M) {
        if (tp.find(m.first.y) != tp.end()) {
            tp[m.first.y].emplace_back(m);
        } else {
            tp[m.first.y] = {m};
        }
    }

    vector<pair<int, vector<array<int, 2>>>> result = {};
    for (auto& row : tp) {
        if (row.second.empty()) {
            continue;
        }
        std::sort(row.second.begin(), row.second.end(),[](const pair<Point<int>, bool> &i1, const pair<Point<int>, bool> &i2) {
            return (i1.first.x <= i2.first.x);
        });

        vector<array<int, 2>> ranges = {};
        for (size_t i = 0, size = row.second.size(); i < size; ++i) {
            const auto& tile = row.second[i];
            auto lowerBound = tile.first.x;

            i += 1;
            std::pair<Point<int>, bool> tempTile = tile;
            while ((i < size) && (tempTile.first.x == row.second[i].first.x - 1)) {
                tempTile = row.second[i];
                i += 1;
            }
            if (i < size) {
                tempTile = row.second[i];
                // merge tails
                while ((i + 1 < size) && (tempTile.first.x == row.second[i + 1].first.x - 1)) {
                    i += 1;
                    tempTile = row.second[i];
                }
            }

            auto upperBound = tempTile.first.x;
            ranges.emplace_back(array<int, 2>{lowerBound, upperBound});
        }

        result.emplace_back(row.first, ranges);
    }

    return result;
}

array<vector<pair<int, vector<array<int, 2>>>>, 8> QueryIncludedTilesByRow(
    const vector<array<double, 2>>  &polygon,
    const double                    grid
) {
    // confirm that given polygon is at least a triangle
    if (polygon.size() < 4) {
        return {};
    }
    vector<Segment> segments = {};

    // append edges
    for (size_t i = 1; i < polygon.size(); i++) {
        segments.emplace_back(Segment(
            {PointF(polygon[i - 1]), PointF(polygon[i])}
        ));
    }
    auto&& M = FindBorderTiles(segments, grid);

    // needn't for merging results (multi-ring mode)
    // MergingTiles(M...)

    // make range by row/column
    auto&& ranges = makeCellRangesHorizontal(M);

    array<vector<pair<int, vector<array<int, 2>>>>, 8> result = {};
    for (const auto& range : ranges) {
        auto file_index_row = range.first / gridSize * 4;
        for (const auto& r : range.second) {
            auto file_index_col_start = r[0] / gridSize;
            auto file_index_col_end = r[1] / gridSize;
            for (auto i = file_index_col_start; i <= file_index_col_end; ++i) {
                auto rel_row = std::max(0, r[0] - i * gridSize);
                auto rel_col = std::max(0, r[1] - i * gridSize);

                if (!result[file_index_row + i].empty() && result[file_index_row + i].rbegin()->first == range.first) {
                    result[file_index_row + i].rbegin()->second.emplace_back(array<int, 2>{rel_row, rel_col});
                } else {
                    result[file_index_row + i].emplace_back(range.first, vector<array<int, 2>>{{rel_row, rel_col}});
                }
            }
        }
    }

    return result;
}
