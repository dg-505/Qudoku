#include "sudoku/NakedTriple.h"
//#include "sudoku/Field.h"

namespace sudoku
{
    NakedTriple::NakedTriple(Field* field1, Field* field2, Field* field3, uint8_t cand1, uint8_t cand2, uint8_t cand3, const std::string& type)
        : _field1(field1), _field2(field2), _field3(field3), _candidate1(cand1), _candidate2(cand2), _candidate3(cand3), _type(type), _unitNumber(-1)
    {
        if (type == "Row")
            _unitNumber = *_field1->getRID();
        else if (type == "Col")
            _unitNumber = *_field1->getCID();
        else if (type == "Block")
            _unitNumber = *_field1->getBID();
    }

    Field* NakedTriple::getField1()
    {
        return _field1;
    }

    void NakedTriple::setField1(Field* field1)
    {
        _field1 = field1;
    }

    Field* NakedTriple::getField2()
    {
        return _field2;
    }

    void NakedTriple::setField2(Field* field2)
    {
        _field2 = field2;
    }

    Field* NakedTriple::getField3()
    {
        return _field3;
    }

    void NakedTriple::setField3(Field* field3)
    {
        _field3 = field3;
    }

    uint8_t NakedTriple::getCandidate1()
    {
        return _candidate1;
    }

    void NakedTriple::setCandidate1(uint8_t cand1)
    {
        _candidate1 = cand1;
    }

    uint8_t NakedTriple::getCandidate2()
    {
        return _candidate2;
    }

    void NakedTriple::setCandidate2(uint8_t cand2)
    {
        _candidate2 = cand2;
    }

    uint8_t NakedTriple::getCandidate3()
    {
        return _candidate3;
    }

    void NakedTriple::setCandidate3(uint8_t cand3)
    {
        _candidate3 = cand3;
    }

    std::string NakedTriple::getType()
    {
        return _type;
    }

    void NakedTriple::setType(const std::string& type)
    {
        _type = type;
    }

    uint8_t NakedTriple::getUnitNumber()
    {
        return _unitNumber;
    }

    void NakedTriple::setUnitNumber(uint8_t num)
    {
        _unitNumber = num;
    }
} // namespace sudoku
