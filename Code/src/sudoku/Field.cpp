#include <QtCore/QStringList>

#include "globals.h"
#include "sudoku/Field.h"

namespace sudoku
{
    // Default constructor
    Field::Field()
        : _fID(), _rID(), _cID(), _bID() {}

    // Konstruktor: besetzt ein Feld f mit den zugehoerigen IDs und dem Wert val
    Field::Field(const uint8_t fID, const uint8_t val)
        : _fID(fID),
          _rID(getRIDfromFID(fID)),
          _cID(getCIDfromFID(fID)),
          _bID(getBIDfromFID(fID)),
          _val(val) {}

    // assignment operator=
    auto Field::operator=(const Field& other) -> Field&
    {
        if (this != &other)
        {
            _fID = other._fID;
            _rID = other._rID;
            _cID = other._cID;
            _bID = other._bID;
            _candidates = other._candidates;
            _val = other._val;
        }
        return *this;
    }

    // Row-ID, Col-ID und Block-ID aus Feld-ID bestimmen
    auto Field::getRIDfromFID(const uint8_t fID) -> uint8_t
    {
        return (fID - 1) / global::order + 1;
    }

    auto Field::getCIDfromFID(const uint8_t fID) -> uint8_t
    {
        return (fID - 1) % global::order + 1;
    }

    auto Field::getBIDfromFID(const uint8_t fID) -> uint8_t
    {
        return ((getRIDfromFID(fID) - 1) / 3) * 3 + (getCIDfromFID(fID) - 1) / 3 + 1;
    }

    // Access member variables
    auto Field::getFID() const -> const uint8_t*
    {
        return &_fID;
    }

    auto Field::getRID() const -> const uint8_t*
    {
        return &_rID;
    }

    auto Field::getCID() const -> const uint8_t*
    {
        return &_cID;
    }

    auto Field::getBID() const -> const uint8_t*
    {
        return &_bID;
    }

    auto Field::getCandidates() -> std::vector<uint8_t>*
    {
        return &_candidates;
    }

    auto Field::getVal() const -> const uint8_t*
    {
        return &_val;
    }

    void Field::setCandidates(const std::vector<uint8_t>& cands)
    {
        _candidates = cands;
    }

    void Field::setVal(const uint8_t val)
    {
        _val = val;
    }

    void Field::printField(QLogTextBrowser& logTextArea) const
    {
        QStringList candidatesList;
        candidatesList.reserve(global::order);
#pragma unroll static_cast<short>(global::order)
        for (const uint8_t cand : _candidates)
        {
            candidatesList.append(QString::number(cand, global::base));
        }
        logTextArea.append(QStringLiteral("Field-ID ") + QString::number(_fID, global::base) +
                           QStringLiteral(": (") + QString::number(_rID, global::base) +
                           QStringLiteral(",") + QString::number(_cID, global::base) +
                           QStringLiteral(") : ") + QString::number(_val, global::base) +
                           QStringLiteral(" , [") + candidatesList.join(QStringLiteral(", ")) + QStringLiteral("]"));
    }
} // namespace sudoku
