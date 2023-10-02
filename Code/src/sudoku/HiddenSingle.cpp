#include "sudoku/HiddenSingle.h"
#include "sudoku/Sudoku.h"

namespace sudoku
{
    HiddenSingle::HiddenSingle(Field* field, uint8_t cand, const std::string& type)
        : _field(field), _candidate(cand), _type(type), _unitNumber(Sudoku::getUnitNumber(field, type)) {}

    auto HiddenSingle::getField() -> Field*
    {
        return _field;
    }

    void HiddenSingle::setField(Field* field)
    {
        _field = field;
    }

    auto HiddenSingle::getCandidate() const -> const uint8_t*
    {
        return &_candidate;
    }

    void HiddenSingle::setCandidate(const uint8_t cand)
    {
        _candidate = cand;
    }

    auto HiddenSingle::getType() const -> const std::string*
    {
        return &_type;
    }

    void HiddenSingle::setType(const std::string& type)
    {
        _type = type;
    }

    auto HiddenSingle::getUnitNumber() -> uint8_t*
    {
        return &_unitNumber;
    }

    void HiddenSingle::setUnitNumber(const uint8_t num)
    {
        _unitNumber = num;
    }
} // namespace sudoku
