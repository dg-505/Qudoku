#pragma once

#include <string>
#include <vector>

#include "sudoku/Field.h"

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
            HiddenSubset(const std::vector<Field*>& fields, const std::vector<uint8_t>& cand, const std::string& type);

            [[nodiscard]] auto getFields() const -> std::vector<Field*>;
            void setFields(const std::vector<Field*>& field);
            [[nodiscard]] auto getCandidates() const -> std::vector<uint8_t>;
            void setCandidates(const std::vector<uint8_t>& cand);
            [[nodiscard]] auto getType() const -> std::string;
            void setType(const std::string& type);
            [[nodiscard]] auto getUnitNumber() const -> uint8_t;
            void setUnitNumber(uint8_t num);
    };
} // namespace sudoku
