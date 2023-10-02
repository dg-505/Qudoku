#pragma once

#include <string>

#include "sudoku/Field.h"

namespace sudoku
{
    class HiddenSingle
    {
        private:
            Field* _field;
            uint8_t _candidate;
            std::string _type;
            uint8_t _unitNumber;

        public:
            HiddenSingle(Field* field, uint8_t cand, const std::string& type);

            auto getField() -> Field*;
            void setField(Field* field);
            [[nodiscard]] auto getCandidate() const -> const uint8_t*;
            void setCandidate(uint8_t cand);
            [[nodiscard]] auto getType() const -> const std::string*;
            void setType(const std::string& type);
            auto getUnitNumber() -> uint8_t*;
            void setUnitNumber(uint8_t num);
            void getUnitNumber(const std::string& type);
    };
} // namespace sudoku
