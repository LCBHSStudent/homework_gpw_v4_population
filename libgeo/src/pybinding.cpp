//
// Created by tusimple on 2022/1/5.
//

#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

#include "libgeo/cal_grid_row.h"

namespace py = pybind11;

PYBIND11_MODULE(libgeo, m) { // NOLINT
    m.def(
        "query_included_tiles",
        &QueryIncludedTilesByRow,
        py::arg("polygon"), py::arg("grid") = 0.008333333333333333f
    );
}