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
            std::vector<Field*> _fields;
            std::vector<uint8_t> _candidates;
            uint8_t _foundInRunNo;
            std::string _foundByType;

        public:
            Step(const std::array<std::array<Field, global::order>, global::order>& grid, const std::vector<Field*>& fields, const std::vector<uint8_t>& candidates, uint8_t foundInRunNo, const std::string& foundByType);

                Step(const Step&) = default;
                Step(Step&&) = delete;
                auto operator=(const Step&) -> Step& = delete;
                auto operator=(Step&&) -> Step& = delete;

                ~Step() = default;

                auto getGrid() -> std::array<std::array<Field, global::order>, global::order>*;
                auto getFields() -> std::vector<Field*>*;
                auto getCandidates() -> std::vector<uint8_t>*;
                auto getFoundInRunNo() -> uint8_t*;
                auto getFoundByType() -> std::string*;
    };
} // namespace sudoku
