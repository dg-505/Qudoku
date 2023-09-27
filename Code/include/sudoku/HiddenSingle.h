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
            Field* getField();
            void setField(Field* field);
            const uint8_t* getCandidate() const;
            void setCandidate(uint8_t cand);
            const std::string* getType() const;
            void setType(const std::string& type);
            uint8_t* getUnitNumber();
            void setUnitNumber(uint8_t num);
    };
} // namespace sudoku
