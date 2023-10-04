#include "sudoku/Step.h"

namespace sudoku
{
    Step::Step(const std::array<std::array<Field, global::order>, global::order>& grid, const uint8_t foundInRunNo, const std::string& foundByType)
        : _grid(grid), _foundInRunNo(foundInRunNo), _foundByType(foundByType) {}

    auto Step::getGrid() -> std::array<std::array<Field, global::order>, global::order>*
    {
        return &_grid;
    }

    auto Step::getFoundInRunNo() -> uint8_t*
    {
        return &_foundInRunNo;
    }

    auto Step::getFoundByType() -> std::string*
    {
        return &_foundByType;
    }
} // namespace sudoku