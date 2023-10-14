#include "sudoku/Step.h"

namespace sudoku
{
    Step::Step(const std::array<std::array<Field, global::order>, global::order>& grid, const std::vector<Field*>& fields, const std::vector<uint8_t>& candidates, const uint8_t foundInRunNo, std::string foundByType)
        : _grid(grid), _fields(fields), _candidates(candidates), _foundInRunNo(foundInRunNo), _foundByType(std::move(foundByType)) {}

    auto Step::getGrid() -> std::array<std::array<Field, global::order>, global::order>*
    {
        return &_grid;
    }

    auto Step::getFields() -> std::vector<Field*>*
    {
        return &_fields;
    }

    auto Step::getCandidates() -> std::vector<uint8_t>*
    {
        return &_candidates;
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
