#pragma once

#include "sudoku/Field.h"
#include <string>

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
            NakedTriple(Field* field1, Field* feld2, Field* field3, uint8_t cand1, uint8_t c2, uint8_t c3, const std::string& type);
            Field* getField1();
            void setField1(Field* field1);
            Field* getField2();
            void setField2(Field* field2);
            Field* getField3();
            void setField3(Field* field3);
            uint8_t getCandidate1();
            void setCandidate1(uint8_t cand1);
            uint8_t getCandidate2();
            void setCandidate2(uint8_t cand2);
            uint8_t getCandidate3();
            void setCandidate3(uint8_t cand3);
            std::string getType();
            void setType(const std::string& type);
            uint8_t getUnitNumber();
            void setUnitNumber(uint8_t num);
    };
} // namespace sudoku
