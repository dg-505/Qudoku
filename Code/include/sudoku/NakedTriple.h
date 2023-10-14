#pragma once

#include <string>

#include "sudoku/Field.h"

namespace sudoku
{
    class NakedTriple
    {
        private:
            Field* _field1;
            Field* _field2;
            Field* _field3;
            uint8_t _candidate1;
            uint8_t _candidate2;
            uint8_t _candidate3;
            std::string _type;
            uint8_t _unitNumber;

        public:
            NakedTriple(Field* field1, Field* field2, Field* field3, uint8_t cand1, uint8_t cand2, uint8_t cand3, const std::string& type);

            auto getField1() -> Field*;
            // void setField1(Field* field1);
            auto getField2() -> Field*;
            // void setField2(Field* field2);
            auto getField3() -> Field*;
            // void setField3(Field* field3);
            [[nodiscard]] auto getCandidate1() const -> uint8_t;
            // void setCandidate1(uint8_t cand1);
            [[nodiscard]] auto getCandidate2() const -> uint8_t;
            // void setCandidate2(uint8_t cand2);
            [[nodiscard]] auto getCandidate3() const -> uint8_t;
            // void setCandidate3(uint8_t cand3);
            auto getType() -> std::string;
            // void setType(const std::string& type);
            [[nodiscard]] auto getUnitNumber() const -> uint8_t;
            // void setUnitNumber(uint8_t num);
    };
} // namespace sudoku
