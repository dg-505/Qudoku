#include "sudoku/NakedTriple.h"
#include "sudoku/Sudoku.h"

namespace sudoku
{
    NakedTriple::NakedTriple(Field* field1, Field* field2, Field* field3, uint8_t cand1, uint8_t cand2, uint8_t cand3, const std::string& type)
        : _field1(field1), _field2(field2), _field3(field3), _candidate1(cand1), _candidate2(cand2), _candidate3(cand3), _type(type), _unitNumber(Sudoku::getUnitNumber(field1, type)) {}

    auto NakedTriple::getField1() -> Field*
    {
        return _field1;
    }

    void NakedTriple::setField1(Field* field1)
    {
        _field1 = field1;
    }

    auto NakedTriple::getField2() -> Field*
    {
        return _field2;
    }

    void NakedTriple::setField2(Field* field2)
    {
        _field2 = field2;
    }

    auto NakedTriple::getField3() -> Field*
    {
        return _field3;
    }

    void NakedTriple::setField3(Field* field3)
    {
        _field3 = field3;
    }

    auto NakedTriple::getCandidate1() const -> uint8_t
    {
        return _candidate1;
    }

    void NakedTriple::setCandidate1(const uint8_t cand1)
    {
        _candidate1 = cand1;
    }

    auto NakedTriple::getCandidate2() const -> uint8_t
    {
        return _candidate2;
    }

    void NakedTriple::setCandidate2(const uint8_t cand2)
    {
        _candidate2 = cand2;
    }

    auto NakedTriple::getCandidate3() const -> uint8_t
    {
        return _candidate3;
    }

    void NakedTriple::setCandidate3(const uint8_t cand3)
    {
        _candidate3 = cand3;
    }

    auto NakedTriple::getType() -> std::string
    {
        return _type;
    }

    void NakedTriple::setType(const std::string& type)
    {
        _type = type;
    }

    auto NakedTriple::getUnitNumber() const -> uint8_t
    {
        return _unitNumber;
    }

    void NakedTriple::setUnitNumber(const uint8_t num)
    {
        _unitNumber = num;
    }
} // namespace sudoku
