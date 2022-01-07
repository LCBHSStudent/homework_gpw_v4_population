#include "libgeo/segment_intersect.h"
#include "libgeo/cal_grid_row.h"

#include <cassert>
#include <iostream>

void TestSegmentIntersect() {
    std::cerr << "Start TestSegmentIntersect\n...\n";
    const Segment base({20, 100}, {500, 100});
    std::array<Segment, 5> testCases = {
        Segment({{351, 232}, {200, 200}}),
        Segment({{274, 5}, {200, 200}}),
        Segment({{641, 81}, {200, 200}}),
        Segment({{338, 477}, {200, 200}}),
        Segment({{311, 99.999999999}, {200, 200}}),
    };
    std::array <bool, 5> expected = {
        false, true, false, false, true
    };

    for (int i = 0; i < 5; i++) {
        assert(isSegmentIntersect(testCases[i], base) == expected[i]);
    }
    std::cerr << "TestSegmentIntersect finished!\n\n";
}

void TestQuery() {
    std::cerr << "Start TestQuery\n...\n";

    std::vector<std::array<double, 2>> data = {
        {281.6455078125, 62.31647191621224},
        {294.78515625, 62.31647191621224},
        {294.78515625, 54.790278354778614},
        {281.6455078125, 54.790278354778614},
        {281.6455078125, 62.31647191621224}
    };
    // PASS
    // QueryIncludedTilesByRow(data);

    std::vector<std::array<double, 2>> data2 = {
        {291.2255859375, 49.8547107043234},
        {286.69921875, 52.07313239851865},
        {287.0068359375, 53.508026529406315},
        {288.017578125, 53.89762355126356},
        {287.2265625, 54.61095003308833},
        {285.6005859375, 54.61095003308833},
        {285.9521484375, 57.34212426304472},
        {288.984375, 58.57133688264139},
        {291.62109375, 57.97329370666386},
        {291.4892578125, 55.43914063291616},
        {291.0498046875, 53.93313786742112},
        {291.62109375, 50.259013644116436},
        {291.2255859375, 49.8547107043234}
    };
    auto result = QueryIncludedTilesByRow(data2);

    std::cerr << "TestQuery finished!\n\n";
}

int main(int, char*[]) {
    TestSegmentIntersect();
    TestQuery();

    return 0;
}
