#pragma once

#include <string>

#include "Field.h"
#include "globals.h"

namespace sudoku
{
    class Step
    {
        private:
            std::array<std::array<Field, global::order>, global::order> _grid;
            uint8_t _foundInRunNo;
            std::string _foundByType;

        public:
                Step(const std::array<std::array<Field, global::order>, global::order>& grid, uint8_t foundInRunNo, const std::string& foundByType);

                Step(const Step&) = default;
                Step(Step&&) = delete;
                auto operator=(const Step&) -> Step& = delete;
                auto operator=(Step&&) -> Step& = delete;

                ~Step() = default;

                auto getGrid() -> std::array<std::array<Field, global::order>, global::order>*;
                auto getFoundInRunNo() -> uint8_t*;
                auto getFoundByType() -> std::string*;
    };
} // namespace sudoku
