#pragma once

#include <QtCore/QString>
//#include <vector>

#include "gui/QLogTextBrowser.h"

namespace sudoku
{
    class Field
    {
        private:
            uint8_t _fID;                     // Nummer des Feldes
            uint8_t _rID;                     // Zeilennummer des Feldes
            uint8_t _cID;                     // Spaltennummer des Feldes
            uint8_t _bID;                     // Blocknummer des Feldes
            std::vector<uint8_t> _candidates; // Moegliche Kandidaten
            uint8_t _val = 0;                 // Wert des Feldes

        public:
            // Default constructor
            Field();

            // Konstruktor: leeres Feld (mit ID, aber ohne Wert)
            explicit Field(uint8_t fID);

            // Konstruktor: besetzt ein Feld f mit den zugehoerigen IDs und dem Wert val
            Field(uint8_t fID, uint8_t val);

            // copy constructor
            Field(const Field& other);

            // Destructor
            ~Field() = default;

            Field& operator=(const Field& other);

            // void setField(const uint8_t f, const uint8_t v);

            // Row-ID, Col-ID und Block-ID aus Feld-ID bestimmen
            [[nodiscard]] const uint8_t getRIDfromFID(uint8_t fID) const;
            [[nodiscard]] const uint8_t getCIDfromFID(uint8_t fID) const;
            [[nodiscard]] const uint8_t getBIDfromFID(uint8_t fID) const;

            // Getter
            [[nodiscard]] const uint8_t* getFID() const;
            [[nodiscard]] const uint8_t* getRID() const;
            [[nodiscard]] const uint8_t* getCID() const;
            [[nodiscard]] const uint8_t* getBID() const;
            std::vector<uint8_t>* getCandidates();
            [[nodiscard]] const uint8_t* getVal() const;

            // set candidates and value
            void setCandidates(const std::vector<uint8_t>* cands);
            void setVal(const uint8_t* val);

            const void printField(QLogTextBrowser& logTextArea) const;
    };
} // namespace sudoku
