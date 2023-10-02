#include "sudoku/HiddenSubset.h"
#include "sudoku/Sudoku.h"

namespace sudoku
{
    HiddenSubset::HiddenSubset(const std::vector<Field*>& fields, const std::vector<uint8_t>& cand, const std::string& type)
        : _fields(fields), _candidates(cand), _type(type), _unitNumber(Sudoku::getUnitNumber(fields.at(0), type)) {}

    auto HiddenSubset::getFields() const -> std::vector<Field*>
    {
        return _fields;
    }

    void HiddenSubset::setFields(const std::vector<Field*>& field)
    {
        _fields = field;
    }

    auto HiddenSubset::getCandidates() const -> std::vector<uint8_t>
    {
        return _candidates;
    }

    void HiddenSubset::setCandidates(const std::vector<uint8_t>& cand)
    {
        _candidates = cand;
    }

    auto HiddenSubset::getType() const -> std::string
    {
        return _type;
    }

    void HiddenSubset::setType(const std::string& type)
    {
        _type = type;
    }

    auto HiddenSubset::getUnitNumber() const -> uint8_t
    {
        return _unitNumber;
    }

    void HiddenSubset::setUnitNumber(const uint8_t num)
    {
        _unitNumber = num;
    }
} // namespace sudoku
