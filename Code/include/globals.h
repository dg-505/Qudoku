#pragma once

#include <cstdint>
#include <string>

namespace global
{
    // version
    const std::string version = "1.2.1";

    // order of the sudoku (9 x 9)
    constexpr uint8_t order = 9;

    constexpr uint16_t fieldDim = 56;
    constexpr uint16_t candDim = 18;

} // namespace global
