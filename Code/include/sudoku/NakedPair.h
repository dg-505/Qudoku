#pragma once

#include "sudoku/Field.h"
#include <string>

namespace sudoku
{
    class NakedPair
    {
        private:
            Field* _field1;
            Field* _field2;
            uint8_t _candidate1;
            uint8_t _candidate2;
            std::string _type;
            uint8_t _unitNumber;

        public:
            NakedPair(Field* field1, Field* field2, uint8_t cand1, uint8_t cand2, const std::string& type);
            Field* getField1();
            void setField1(Field* field1);
            Field* getField2();
            void setField2(Field* field2);
            uint8_t getCandidate1();
            void setCandidate1(uint8_t cand1);
            uint8_t getCandidate2();
            void setCandidate2(uint8_t cand2);
            std::string getType();
            void setType(const std::string& type);
            uint8_t getUnitNumber();
            void setUnitNumber(uint8_t num);
    };
} // namespace sudoku
