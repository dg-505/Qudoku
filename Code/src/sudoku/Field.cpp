#include <QtCore/QStringList>
#include <iomanip>
#include <iostream>

#include "globals.h"
#include "sudoku/Field.h"

using std::cout;
using std::endl;

namespace sudoku
{
    // Default constructor
    Field::Field()
        : _fID(), _rID(), _cID(), _bID()
    {
    }

    // Konstruktor: leeres Feld (mit ID, aber ohne Wert)
    Field::Field(const uint8_t fID)
        : _fID(fID),
          _rID(getRIDfromFID(fID)),
          _cID(getCIDfromFID(fID)),
          _bID(getBIDfromFID(fID))
    {
    }

    // Konstruktor: besetzt ein Feld f mit den zugehoerigen IDs und dem Wert val
    Field::Field(const uint8_t fID, const uint8_t val)
        : _fID(fID),
          _rID(getRIDfromFID(fID)),
          _cID(getCIDfromFID(fID)),
          _bID(getBIDfromFID(fID)),
          _val(val)
    {
        // cout << "Field: val=" << _val << endl;
        // cout << "F: " << this << " | ";
        // cout << "" << std::fixed << std::setw(2) << _fID << " @ " << &_fID << " | ";
        // cout << "" << _rID << " @ " << &_rID << " | ";
        // cout << "" << _cID << " @ " << &_cID << " | ";
        // cout << "" << _bID << " @ " << &_bID << " | ";
        // cout << "" << this->candidates2QString().toStdString() << " @ " << &_candidates << " | ";
        // cout << "" << std::fixed << std::setw(2) << _val << " @ " << &_val << " | ";
        // cout << endl;
    }

    // Copy constructor
    Field::Field(const Field& other)
        : _fID(other._fID),
          _rID(other._rID),
          _cID(other._cID),
          _bID(other._bID),
          _candidates(other._candidates),
          _val(other._val)
    {
    }

    // assignment operator=
    Field& Field::operator=(const Field& other)
    {
        if (this != &other)
        {
            _fID = other._fID; // NOT NEEDED
            _rID = other._rID; // NOT NEEDED
            _cID = other._cID; // NOT NEEDED
            _bID = other._bID; // NOT NEEDED
            _candidates = other._candidates;
            _val = other._val;
        }
        return *this;
    }

    // void Field::setField(const uint8_t f, const uint8_t v)

    // Row-ID, Col-ID und Block-ID aus Feld-ID bestimmen
    const uint8_t Field::getRIDfromFID(uint8_t fID) const
    {
        return (fID - 1) / order + 1;
    }
    const uint8_t Field::getCIDfromFID(uint8_t fID) const
    {
        return (fID - 1) % order + 1;
    }
    const uint8_t Field::getBIDfromFID(uint8_t fID) const
    {
        return ((getRIDfromFID(fID) - 1) / 3) * 3 + (getCIDfromFID(fID) - 1) / 3 + 1;
    }

    // Access member variables
    const uint8_t* Field::getFID() const
    {
        return &_fID;
    }

    const uint8_t* Field::getRID() const
    {
        return &_rID;
    }

    const uint8_t* Field::getCID() const
    {
        return &_cID;
    }

    const uint8_t* Field::getBID() const
    {
        return &_bID;
    }

    std::vector<uint8_t>* Field::getCandidates()
    {
        return &_candidates;
    }

    const uint8_t* Field::getVal() const
    {
        // cout << "Field: val = " << _val << endl;
        return &_val;
    }

    void Field::setCandidates(const std::vector<uint8_t>* cands)
    {
        _candidates = *cands;
    }

    void Field::setVal(const uint8_t* val)
    {
        _val = *val;
    }

    const void Field::printField(QLogTextBrowser& logTextArea) const
    {
        QStringList candidatesList;
        for (const uint8_t c : _candidates)
        {
            candidatesList.append(QString::number(c));
        }
        logTextArea.append(QStringLiteral("Field-ID %1: (%2,%3) : %4 , %5")
                               .arg(_fID)
                               .arg(_rID)
                               .arg(_cID)
                               .arg(_val)
                               .arg("[" + candidatesList.join(QStringLiteral(", ")) + "]"));
    }
} // namespace sudoku
