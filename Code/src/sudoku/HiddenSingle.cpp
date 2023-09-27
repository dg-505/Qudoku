#include "sudoku/HiddenSingle.h"

namespace sudoku
{
    HiddenSingle::HiddenSingle(Field* field, uint8_t cand, const std::string& type)
        : _field(field), _candidate(cand), _type(type), _unitNumber(-1)
    {
        if (type == "Row")
            _unitNumber = *_field->getRID();
        else if (type == "Col")
            _unitNumber = *_field->getCID();
        else if (type == "Block")
            _unitNumber = *_field->getBID();
    }

    Field* HiddenSingle::getField()
    {
        return _field;
    }

    void HiddenSingle::setField(Field* field)
    {
        _field = field;
    }

    const uint8_t* HiddenSingle::getCandidate() const
    {
        return &_candidate;
    }

    void HiddenSingle::setCandidate(uint8_t cand)
    {
        _candidate = cand;
    }

    const std::string* HiddenSingle::getType() const
    {
        return &_type;
    }

    void HiddenSingle::setType(const std::string& type)
    {
        _type = type;
    }

    uint8_t* HiddenSingle::getUnitNumber()
    {
        return &_unitNumber;
    }

    void HiddenSingle::setUnitNumber(uint8_t num)
    {
        _unitNumber = num;
    }
} // namespace sudoku
