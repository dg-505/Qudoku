#pragma once

#include "sudoku/Field.h"
//#include <list>
#include <string>
#include <vector>

namespace sudoku
{
    class HiddenSubset
    {
        private:
            std::vector<Field*> _fields;
            std::vector<uint8_t> _candidates;
            std::string _type;
            uint8_t _unitNumber;

        public:
            HiddenSubset(const std::vector<Field*>& field, const std::vector<uint8_t>& cand, const std::string& type);
            std::vector<Field*> getFields() const;
            void setFields(const std::vector<Field*>& field);
            std::vector<uint8_t> getCandidates() const;
            void setCandidates(const std::vector<uint8_t>& cand);
            std::string getType() const;
            void setType(const std::string& type);
            uint8_t getUnitNumber() const;
            void setUnitNumber(uint8_t num);
    };
}
