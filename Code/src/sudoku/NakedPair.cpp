#include "sudoku/NakedPair.h"
#include "sudoku/Sudoku.h"

namespace sudoku
{
    NakedPair::NakedPair(Field* field1, Field* field2, uint8_t cand1, uint8_t cand2, const std::string& type)
        : _field1(field1), _field2(field2), _candidate1(cand1), _candidate2(cand2), _type(type), _unitNumber(Sudoku::getUnitNumber(field1, type)) {}

    auto NakedPair::getField1() -> Field*
    {
        return _field1;
    }

/*
    void NakedPair::setField1(Field* field1)
    {
        _field1 = field1;
    }
*/

    auto NakedPair::getField2() -> Field*
    {
        return _field2;
    }

/*
    void NakedPair::setField2(Field* field2)
    {
        _field2 = field2;
    }
*/

    auto NakedPair::getCandidate1() const -> uint8_t
    {
        return _candidate1;
    }

/*
    void NakedPair::setCandidate1(const uint8_t cand1)
    {
        _candidate1 = cand1;
    }
*/

    auto NakedPair::getCandidate2() const -> uint8_t
    {
        return _candidate2;
    }

/*
    void NakedPair::setCandidate2(const uint8_t cand2)
    {
        _candidate2 = cand2;
    }
*/

    auto NakedPair::getType() -> std::string
    {
        return _type;
    }

/*
    void NakedPair::setType(const std::string& type)
    {
        _type = type;
    }
*/

    auto NakedPair::getUnitNumber() const -> uint8_t
    {
        return _unitNumber;
    }

/*
    void NakedPair::setUnitNumber(const uint8_t num)
    {
        _unitNumber = num;
    }
*/
} // namespace sudoku
