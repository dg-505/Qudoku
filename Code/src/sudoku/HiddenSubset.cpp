#include "sudoku/HiddenSubset.h"

namespace sudoku
{
    HiddenSubset::HiddenSubset(const std::vector<Field*>& field, const std::vector<uint8_t>& cand, const std::string& type)
        : _fields(field), _candidates(cand), _type(type), _unitNumber(-1)
    {
        if (type == "Row")
            _unitNumber = *_fields[0]->getRID();
        else if (type == "Col")
            _unitNumber = *_fields[0]->getCID();
        else if (type == "Block")
            _unitNumber = *_fields[0]->getBID();
    }

    std::vector<Field*> HiddenSubset::getFields() const
    {
        return _fields;
    }

    void HiddenSubset::setFields(const std::vector<Field*>& field)
    {
        _fields = field;
    }

    std::vector<uint8_t> HiddenSubset::getCandidates() const
    {
        return _candidates;
    }

    void HiddenSubset::setCandidates(const std::vector<uint8_t>& cand)
    {
        _candidates = cand;
    }

    std::string HiddenSubset::getType() const
    {
        return _type;
    }

    void HiddenSubset::setType(const std::string& type)
    {
        _type = type;
    }

    uint8_t HiddenSubset::getUnitNumber() const
    {
        return _unitNumber;
    }

    void HiddenSubset::setUnitNumber(uint8_t num)
    {
        _unitNumber = num;
    }
} // namespace sudoku
