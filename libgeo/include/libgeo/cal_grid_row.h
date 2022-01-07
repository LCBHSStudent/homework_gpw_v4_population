//
// Created by tusimple on 2022/1/5.
//

#ifndef LIBGEO_CAL_GRID_ROW_H
#define LIBGEO_CAL_GRID_ROW_H

#include "common_types.h"

#include <array>

/**
 * @brief Query tile ranges of each file block
 * @param polygon Points of polygon in specific order
 */
__attribute__((visibility("default"))) std::array<std::vector<std::pair<int, std::vector<std::array<int, 2>>>>, 8>
QueryIncludedTilesByRow(
    const std::vector<std::array<double, 2>>    &polygon,
    double                                      grid = 0.008333333333333333f
);

#endif //LIBGEO_CAL_GRID_ROW_H
