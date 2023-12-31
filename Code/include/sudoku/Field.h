#pragma once

#include <QString>

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
            Field();
            // Konstruktor: besetzt ein Feld f mit den zugehoerigen IDs und dem Wert val
            Field(uint8_t fID, uint8_t val);
            Field(const Field& other) = default;
            Field(Field&&) = delete;
            auto operator=(const Field& other) -> Field&;
            auto operator=(Field&&) -> Field& = default;

            // Destructor
            ~Field() = default;

            // Row-ID, Col-ID und Block-ID aus Feld-ID bestimmen
            [[nodiscard]] static auto getRIDfromFID(uint8_t fID) -> uint8_t;
            [[nodiscard]] static auto getCIDfromFID(uint8_t fID) -> uint8_t;
            [[nodiscard]] static auto getBIDfromFID(uint8_t fID) -> uint8_t;

            // Getter
            [[nodiscard]] auto getFID() const -> const uint8_t*;
            [[nodiscard]] auto getRID() const -> const uint8_t*;
            [[nodiscard]] auto getCID() const -> const uint8_t*;
            [[nodiscard]] auto getBID() const -> const uint8_t*;
            auto getCandidates() -> std::vector<uint8_t>*;
            [[nodiscard]] auto getVal() const -> const uint8_t*;

            // set candidates and value
            void setCandidates(const std::vector<uint8_t>& cands);
            void setVal(uint8_t val);

            void printField(QLogTextBrowser& logTextArea) const;
    };
} // namespace sudoku
