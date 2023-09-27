#include "sudoku/NakedPair.h"
//#include "sudoku/Field.h"

namespace sudoku
{
    NakedPair::NakedPair(Field* field1, Field* field2, uint8_t cand1, uint8_t cand2, const std::string& type)
        : _field1(field1), _field2(field2), _candidate1(cand1), _candidate2(cand2), _type(type), _unitNumber(-1)
    {
        if (type == "Row")
            _unitNumber = *_field1->getRID();
        else if (type == "Col")
            _unitNumber = *_field1->getCID();
        else if (type == "Block")
            _unitNumber = *_field1->getBID();
    }

    Field* NakedPair::getField1()
    {
        return _field1;
    }

    void NakedPair::setField1(Field* field1)
    {
        _field1 = field1;
    }

    Field* NakedPair::getField2()
    {
        return _field2;
    }

    void NakedPair::setField2(Field* field2)
    {
        _field2 = field2;
    }

    uint8_t NakedPair::getCandidate1()
    {
        return _candidate1;
    }

    void NakedPair::setCandidate1(uint8_t cand1)
    {
        _candidate1 = cand1;
    }

    uint8_t NakedPair::getCandidate2()
    {
        return _candidate2;
    }

    void NakedPair::setCandidate2(uint8_t cand2)
    {
        _candidate2 = cand2;
    }

    std::string NakedPair::getType()
    {
        return _type;
    }

    void NakedPair::setType(const std::string& type)
    {
        _type = type;
    }

    uint8_t NakedPair::getUnitNumber()
    {
        return _unitNumber;
    }

    void NakedPair::setUnitNumber(uint8_t num)
    {
        _unitNumber = num;
    }
} // namespace sudoku
