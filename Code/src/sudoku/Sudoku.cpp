#include <QString>
#include <QTextStream>
#include <utility>

#include "sudoku/Sudoku.h"

namespace sudoku
{
    Sudoku::Sudoku(std::string  name, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>* vals, QLogTextBrowser* logTextArea, const bool nakedSinglesEnabled, const bool hiddenSinglesEnabled, const bool nakedPairsEnabled, const bool hiddenPairsEnabled, const bool nakedTriplesEnabled, const bool hiddenTriplesEnabled, const bool blockLineChecksEnabled, const bool lineBlockChecksEnabled, const bool backtrackingEnabled)
        : _name(std::move(name)),
          _logTextArea(logTextArea),
          _useNakedSingles(nakedSinglesEnabled),
          _useHiddenSingles(hiddenSinglesEnabled),
          _useNakedPairs(nakedPairsEnabled),
          _useHiddenPairs(hiddenPairsEnabled),
          _useNakedTriples(nakedTriplesEnabled),
          _useHiddenTriples(hiddenTriplesEnabled),
          _useBlockLineChecks(blockLineChecksEnabled),
          _useLineBlockChecks(lineBlockChecksEnabled),
          _useBacktracking(backtrackingEnabled)
    {
        // Fill values
        uint8_t fID = 1;
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
#pragma unroll static_cast < short>(global::order)
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                _grid.at(rID - 1).at(cID - 1) = Field(fID, vals->at(fID - 1));
                fID++;
            }
        }

        // Fill candidates
        // Initially: 1-9 are possible candidates.
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                Field* field = &_grid.at(rID - 1).at(cID - 1);
                if (*field->getVal() == 0)
                {
                    std::vector<uint8_t> candidates;
                    candidates.reserve(global::order);
#pragma unroll static_cast < short>(global::order)
                    for (uint8_t cand = 1; cand <= global::order; cand++)
                    {
                        candidates.push_back(cand);
                    }
                    field->setCandidates(candidates);
                }
            }
        }

        // Then: For each field.candidates(): go through rows, cols, blocks and
        // remove existing vals from cands

        // rows
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                Field* field = &_grid.at(rID - 1).at(cID - 1);
                std::vector<uint8_t>* candidates = field->getCandidates();

                std::array<Field*, global::order> row = getRowByFieldID(*field->getFID());
#pragma unroll static_cast < short>(global::order)
                for (uint8_t fieldInRow = 1; fieldInRow <= global::order; fieldInRow++)
                {
                    const uint8_t* val = row.at(fieldInRow - 1)->getVal();
                    if (*val != 0)
                    {
                        candidates->erase(std::remove(candidates->begin(), candidates->end(), *val), candidates->end());
                    }
                }
            }
        }

        // cols
        for (uint8_t cID = 1; cID <= global::order; cID++)
        {
            for (uint8_t rID = 1; rID <= global::order; rID++)
            {
                Field* field = &_grid.at(rID - 1).at(cID - 1);
                std::vector<uint8_t>* candidates = field->getCandidates();

                std::array<Field*, global::order> col = getColByFieldID(*field->getFID());
#pragma unroll static_cast < short>(global::order)
                for (uint8_t fieldInCol = 1; fieldInCol <= global::order; fieldInCol++)
                {
                    const uint8_t* val = col.at(fieldInCol - 1)->getVal();
                    if (*val != 0)
                    {
                        candidates->erase(std::remove(candidates->begin(), candidates->end(), *val), candidates->end());
                    }
                }
            }
        }

        // blocks
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                Field* field = &_grid.at(rID - 1).at(cID - 1);
                std::vector<uint8_t>* candidates = field->getCandidates();

                std::array<Field*, global::order> block = getBlockByFieldID(*field->getFID());
#pragma unroll static_cast < short>(global::order)
                for (uint8_t bID = 1; bID <= global::order; bID++)
                {
                    const uint8_t* val = block.at(bID - 1)->getVal();
                    if (*val != 0)
                    {
                        candidates->erase(std::remove(candidates->begin(), candidates->end(), *val), candidates->end());
                    }
                }
            }
        }
    }

    auto Sudoku::getGrid() -> std::array<std::array<Field, global::order>, global::order>*
    {
        return &_grid;
    }

    auto Sudoku::getSteps() -> std::vector<Step>*
    {
        return &_steps;
    }

    /*
    auto Sudoku::getLogTextArea() -> QLogTextBrowser*
    {
        return _logTextArea;
    }

    auto Sudoku::getFieldByCoord(const uint8_t rID, const uint8_t cID) -> Field*
    {
        return &_grid.at(rID - 1).at(cID - 1);
    }

    auto Sudoku::getFieldByFieldID(uint8_t fID) -> Field*
    {
        const uint8_t rID = (fID - 1) / global::order + 1;
        const uint8_t cID = (fID - 1) % global::order + 1;
        return getFieldByCoord(rID, cID);
    }
*/

    auto Sudoku::getRowByRowID(const uint8_t rID) -> std::array<Field*, global::order>
    {
        std::array<Field*, global::order> row{};
#pragma unroll static_cast < short>(global::order)
        for (uint8_t cID = 1; cID <= global::order; cID++)
        {
            row.at(cID - 1) = &_grid.at(rID - 1).at(cID - 1);
        }
        return row;
    }

    auto Sudoku::getRowByFieldID(const uint8_t fID) -> std::array<Field*, global::order>
    {
        const uint8_t rID = (fID - 1) / global::order + 1;
        return getRowByRowID(rID);
    }

    auto Sudoku::getColByColID(const uint8_t cID) -> std::array<Field*, global::order>
    {

        std::array<Field*, global::order> col{};
#pragma unroll static_cast < short>(global::order)
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
            col.at(rID - 1) = &_grid.at(rID - 1).at(cID - 1);
        }
        return col;
    }

    auto Sudoku::getColByFieldID(const uint8_t fID) -> std::array<Field*, global::order>
    {
        const uint8_t cID = (fID - 1) % global::order + 1;
        return getColByColID(cID);
    }

    auto Sudoku::getBlockByBlockID(const uint8_t bID) -> std::array<Field*, global::order>
    {
        std::array<Field*, global::order> block{};

        const uint8_t row0 = (bID - 1) / 3 * 3;
        const uint8_t col0 = (bID - 1) % 3 * 3;

        uint8_t num = 0;
        for (uint8_t row = row0; row < row0 + 3; row++)
        {
#pragma unroll 3
            for (uint8_t col = col0; col < col0 + 3; col++)
            {
                block.at(num) = &_grid.at(row).at(col);
                num++;
            }
        }
        return block;
    }

    auto Sudoku::getBlockByFieldID(const uint8_t fID) -> std::array<Field*, global::order>
    {
        // clang-format off
        if (fID ==  1 || fID ==  2 || fID ==  3 || fID == 10 || fID == 11 || fID == 12 || fID == 19 || fID == 20 || fID == 21) { return getBlockByBlockID(1); } // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        if (fID ==  4 || fID ==  5 || fID ==  6 || fID == 13 || fID == 14 || fID == 15 || fID == 22 || fID == 23 || fID == 24) { return getBlockByBlockID(2); } // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        if (fID ==  7 || fID ==  8 || fID ==  9 || fID == 16 || fID == 17 || fID == 18 || fID == 25 || fID == 26 || fID == 27) { return getBlockByBlockID(3); } // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        if (fID == 28 || fID == 29 || fID == 30 || fID == 37 || fID == 38 || fID == 39 || fID == 46 || fID == 47 || fID == 48) { return getBlockByBlockID(4); } // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        if (fID == 31 || fID == 32 || fID == 33 || fID == 40 || fID == 41 || fID == 42 || fID == 49 || fID == 50 || fID == 51) { return getBlockByBlockID(5); } // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        if (fID == 34 || fID == 35 || fID == 36 || fID == 43 || fID == 44 || fID == 45 || fID == 52 || fID == 53 || fID == 54) { return getBlockByBlockID(6); } // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        if (fID == 55 || fID == 56 || fID == 57 || fID == 64 || fID == 65 || fID == 66 || fID == 73 || fID == 74 || fID == 75) { return getBlockByBlockID(7); } // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        if (fID == 58 || fID == 59 || fID == 60 || fID == 67 || fID == 68 || fID == 69 || fID == 76 || fID == 77 || fID == 78) { return getBlockByBlockID(8); } // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                                                                                                                               { return getBlockByBlockID(9); } // NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        // clang-format on
    }

    auto Sudoku::getFreeFields() -> std::vector<Field*>
    {
        std::vector<Field*> freeFields;
        for (auto& row : _grid)
        {
#pragma unroll static_cast < short>(global::order)
            for (auto& field : row)
            {
                if (*field.getVal() == 0)
                {
                    freeFields.push_back(&field);
                }
            }
        }
        return freeFields;
    }

    auto Sudoku::countCandidates() -> uint16_t
    {
        uint16_t numCands = 0;
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
#pragma unroll static_cast < short>(global::order)
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                numCands += _grid.at(rID - 1).at(cID - 1).getCandidates()->size();
            }
        }
        return numCands;
    }

    void Sudoku::filldAndEliminate(Field* field)
    {
        // set the value of f to val
        if (field->getCandidates()->size() > 1)
        {
            const QString errorMessage = "Error filling Field (" +
                                         QString::number(*field->getRID(), global::base) + "," +
                                         QString::number(*field->getCID(), global::base) +
                                         "): Field has more than one candidate!";
            _logTextArea->append(errorMessage);
            return;
        }
        const uint8_t val = field->getCandidates()->front();
        field->setVal(val);

        // eliminate candidate "val" from units
        auto rowFields = this->getRowByFieldID(*field->getFID());
#pragma unroll static_cast < short>(global::order)
        for (Field* rowField : rowFields)
        {
            rowField->getCandidates()->erase(std::remove(rowField->getCandidates()->begin(), rowField->getCandidates()->end(), val), rowField->getCandidates()->end());
        }

        auto colFields = this->getColByFieldID(*field->getFID());
#pragma unroll static_cast < short>(global::order)
        for (Field* colField : colFields)
        {
            colField->getCandidates()->erase(std::remove(colField->getCandidates()->begin(), colField->getCandidates()->end(), val), colField->getCandidates()->end());
        }

        auto blockFields = this->getBlockByFieldID(*field->getFID());
#pragma unroll static_cast < short>(global::order)
        for (Field* blockField : blockFields)
        {
            blockField->getCandidates()->erase(std::remove(blockField->getCandidates()->begin(), blockField->getCandidates()->end(), val), blockField->getCandidates()->end());
        }

        auto freeFields = this->getFreeFields();
        freeFields.erase(std::remove(freeFields.begin(), freeFields.end(), field), freeFields.end());
    }

    // Naked Single methods
    auto Sudoku::firstNakedSingle() -> Field*
    {
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
#pragma unroll static_cast < short>(global::order)
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                Field* field = &_grid.at(rID - 1).at(cID - 1);
                if (field->getCandidates()->size() == 1)
                {
                    return field;
                }
            }
        }
        return nullptr;
    }

    void Sudoku::processNakedSingles(const uint8_t run)
    {
#pragma unroll
        while (true)
        {
            Field* firstNakedSingle = this->firstNakedSingle();
            if (firstNakedSingle == nullptr)
            {
                break;
            }
            this->filldAndEliminate(firstNakedSingle);
            const std::string msg =
                "NakedSingle {" + std::to_string(*firstNakedSingle->getVal()) +
                "} in Field (" + std::to_string(*firstNakedSingle->getRID()) + "," +
                std::to_string(*firstNakedSingle->getCID()) + ")";
            _steps.emplace_back(_grid, std::vector<Field*>({firstNakedSingle}, std::allocator<Field*>()), std::vector<uint8_t>({firstNakedSingle->getCandidates()->front()}, std::allocator<uint8_t>()), run, msg);
            _logTextArea->append(QString::fromStdString(msg));
        }
        // this->print();
        // this->printFields();
    }

    auto Sudoku::fieldContainsCandidate(std::vector<uint8_t>* candidates, const uint8_t cand) -> bool
    {
        return std::find(candidates->begin(), candidates->end(), cand) != candidates->end();
    }

    auto Sudoku::candidateOccurrencesInUnit(std::array<Field*, global::order> unit) -> std::array<uint8_t, global::order>
    {
        std::array<uint8_t, global::order> candidateOccurrencesInUnit{}; // Initialize array elements to 0
        for (uint8_t cand = 1; cand <= global::order; cand++)
        {
            uint8_t count = 0;
#pragma unroll static_cast < short>(global::order)
            for (uint8_t unitID = 1; unitID <= global::order; unitID++)
            {
                if (fieldContainsCandidate(unit.at(unitID - 1)->getCandidates(), cand))
                {
                    count++;
                }
            }
            candidateOccurrencesInUnit.at(cand - 1) = count;
        }
        return candidateOccurrencesInUnit;
    }

    auto Sudoku::firstHiddenSingle() -> HiddenSingle*
    {
        std::string row;
        row = "Row";
        std::string col;
        col = "Col";
        std::string block;
        block = "Block";
        // Reihenfolge: Erst Reihen, dann Spalten, dann Bloecke
        // nach HiddenSingle durchsuchen
        const std::array<std::string, 3> types{row, col, block};
        for (const std::string& type : types)
        {
            for (uint8_t unitID = 1; unitID <= global::order; unitID++)
            {
                auto unit = this->getUnit(type, unitID);
                std::array<uint8_t, global::order> candsInUnit = candidateOccurrencesInUnit(unit);
                // Finde Zahlen "i", die nur 1x vorkommen
                for (uint8_t i = 1; i <= global::order; i++)
                {
                    if (candsInUnit.at(i - 1) == 1)
                    {
                        // Pruefe Felder in Reihe i: Welches Feld enthaelt Zahl "num"?
#pragma unroll static_cast<short>(global::order)
                        for (uint8_t fID = 1; fID <= global::order; fID++)
                        {
                            Field* field = unit.at(fID - 1);
                            if (fieldContainsCandidate(field->getCandidates(), i))
                            {
                                // Kandidat fuer Feld f ist nur Zahl i
                                return std::make_unique<HiddenSingle>(field, i, type).release();
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    void Sudoku::processHiddenSingles(const uint8_t run)
    {
#pragma unroll
        while (true)
        {
            HiddenSingle* firstHiddenSingle = this->firstHiddenSingle();
            if (firstHiddenSingle == nullptr)
            {
                break;
            }
            const std::vector<uint8_t> cand({*firstHiddenSingle->getCandidate()}, std::allocator<uint8_t>());
            firstHiddenSingle->getField()->setCandidates(cand);
            this->filldAndEliminate(firstHiddenSingle->getField());

            const std::string msg =
                "HiddenSingle {" +
                std::to_string(*firstHiddenSingle->getField()->getVal()) + "} in " +
                *firstHiddenSingle->getType() + " " +
                std::to_string(*firstHiddenSingle->getUnitNumber()) + ": Field (" +
                std::to_string(*firstHiddenSingle->getField()->getRID()) + "," +
                std::to_string(*firstHiddenSingle->getField()->getCID()) + ")";
            _steps.emplace_back(_grid, std::vector<Field*>({firstHiddenSingle->getField()}, std::allocator<Field*>()), std::vector<uint8_t>({*firstHiddenSingle->getCandidate()}, std::allocator<uint8_t>()), run, msg);
            _logTextArea->append(QString::fromStdString(msg));
        }
        // this->print();
        // this->printFields();
    }

    // Naked Pair methods
    auto Sudoku::firstNakedPair(std::vector<NakedPair*>& deadNakedPairs) -> NakedPair*
    {
        std::string row;
        row = "Row";
        std::string col;
        col = "Col";
        std::string block;
        block = "Block";
        const std::array<std::string, 3> types{row, col, block};
        for (const std::string& type : types)
        {
            for (uint8_t unitID = 1; unitID <= global::order; unitID++) // go over all units
            {
                auto unit = this->getUnit(type, unitID);
                uint8_t numFreeFieldsInUnit = 0; // In der jeweiligen Unit muessen > 2 freie Felder sein
#pragma unroll static_cast < short>(global::order)
                for (uint8_t fID = 1; fID <= global::order; fID++)
                {
                    if (*unit.at(fID - 1)->getVal() == 0)
                    {
                        numFreeFieldsInUnit++;
                    }
                }

                for (uint8_t i_1 = 1; i_1 <= global::order - 1; i_1++) // Alle Zahlenpaare {1,2} bis {8,9}
                {
                    for (uint8_t i_2 = i_1 + 1; i_2 <= global::order; i_2++)
                    {
                        for (uint8_t fID1 = 1; fID1 <= global::order - 1; fID1++) // Alle Feld-Kombinationen in unit
                        {
                            for (uint8_t fID2 = fID1 + 1; fID2 <= global::order; fID2++)
                            {
                                Field* field1 = unit.at(fID1 - 1);
                                Field* field2 = unit.at(fID2 - 1);
                                // Kandidatenlisten der beiden Felder muessen Laenge 2 haben und
                                // die einzigen Kandidaten muessen die Zahlen i und j sein
                                std::array<uint8_t, 2> cands{i_1, i_2};
                                if (field1->getCandidates()->size() == 2 &&
                                    field2->getCandidates()->size() == 2 &&
                                    std::equal(field1->getCandidates()->begin(), field1->getCandidates()->end(), cands.begin(), cands.end()) &&
                                    std::equal(field2->getCandidates()->begin(), field2->getCandidates()->end(), cands.begin(), cands.end()) &&
                                    numFreeFieldsInUnit > 2)
                                {
                                    auto* firstNakedPair = std::make_unique<NakedPair>(field1, field2, i_1, i_2, type).release();
                                    bool isDead = false;
#pragma unroll 100
                                    for (NakedPair* dead : deadNakedPairs)
                                    {
                                        if (dead->getField1() == firstNakedPair->getField1() &&
                                            dead->getField2() == firstNakedPair->getField2() &&
                                            dead->getType() == firstNakedPair->getType())
                                        {
                                            isDead = true;
                                        }
                                    }
                                    if (!isDead)
                                    {
                                        deadNakedPairs.push_back(firstNakedPair);
                                        return firstNakedPair;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    void Sudoku::eliminateCandidatesOfNakedPairInUnit(NakedPair* nakedPair, const std::array<Field*, global::order>& unit)
    {
        for (Field* field : unit)
        {
            if (field->getFID() == nakedPair->getField1()->getFID() ||
                field->getFID() == nakedPair->getField2()->getFID() ||
                *field->getVal() != 0)
            {
                continue;
            }

            std::vector<uint8_t>* candidates = field->getCandidates();
            std::vector<uint8_t>* candidatesToRemove = nakedPair->getField1()->getCandidates();

#pragma unroll static_cast < short>(global::order)
            for (const uint8_t candidate : *candidatesToRemove)
            {
                candidates->erase(std::remove(candidates->begin(), candidates->end(), candidate), candidates->end());
            }
        }
    }

    void Sudoku::eliminateNakedPair(NakedPair* nakedPair)
    {
        if (*nakedPair->getField1()->getRID() == *nakedPair->getField2()->getRID())
        {
            eliminateCandidatesOfNakedPairInUnit(nakedPair, getRowByFieldID(*nakedPair->getField1()->getFID()));
        }
        if (*nakedPair->getField1()->getCID() == *nakedPair->getField2()->getCID())
        {
            eliminateCandidatesOfNakedPairInUnit(nakedPair, getColByFieldID(*nakedPair->getField1()->getFID()));
        }
        if (*nakedPair->getField1()->getBID() == *nakedPair->getField2()->getBID())
        {
            eliminateCandidatesOfNakedPairInUnit(nakedPair, getBlockByFieldID(*nakedPair->getField1()->getFID()));
        }
    }

    void Sudoku::processNakedPairs(const uint8_t run)
    {
        std::vector<NakedPair*> deadNakedPairs;
#pragma unroll
        while (true)
        {
            NakedPair* firstNakedPair = this->firstNakedPair(deadNakedPairs);
            if (firstNakedPair == nullptr)
            {
                break;
            }
            const auto numCandsBeforeEliminatingFirstNakedPair = this->countCandidates();
            this->eliminateNakedPair(firstNakedPair);
            if (this->countCandidates() < numCandsBeforeEliminatingFirstNakedPair)
            {
                const std::string msg =
                    "NakedPair {" +
                    std::to_string(firstNakedPair->getCandidate1()) + "," +
                    std::to_string(firstNakedPair->getCandidate2()) + "} in " +
                    firstNakedPair->getType() + " " +
                    std::to_string(firstNakedPair->getUnitNumber()) + ": Fields (" +
                    std::to_string(*firstNakedPair->getField1()->getRID()) + "," +
                    std::to_string(*firstNakedPair->getField1()->getCID()) + ");(" +
                    std::to_string(*firstNakedPair->getField2()->getRID()) + "," +
                    std::to_string(*firstNakedPair->getField2()->getCID()) + ")";
                _steps.emplace_back(_grid, std::vector<Field*>({firstNakedPair->getField1(), firstNakedPair->getField2()}, std::allocator<Field*>()), std::vector<uint8_t>({firstNakedPair->getCandidate1(), firstNakedPair->getCandidate2()}, std::allocator<uint8_t>()), run, msg);
                _logTextArea->append(QString::fromStdString(msg));
            }
        }
        // this->print();
        // this->printFields();
    }

    // Hidden Pair methods
    auto Sudoku::firstHiddenPair() -> HiddenSubset*
    {
        std::string row;
        row = "Row";
        std::string col;
        col = "Col";
        std::string block;
        block = "Block";
        const std::array<std::string, 3> types{row, col, block};
        for (const std::string& type : types)
        {
            for (uint8_t unitID = 1; unitID <= global::order; unitID++) // go over all units
            {
                auto unit = this->getUnit(type, unitID);
                std::array<uint8_t, global::order> candidateOccurrences = candidateOccurrencesInUnit(unit);
                for (uint8_t i_1 = 1; i_1 <= global::order - 1; i_1++) // Alle Zahlenpaare {1,2} bis {8,9}
                {
                    for (uint8_t i_2 = i_1 + 1; i_2 <= global::order; i_2++)
                    {
                        // Wenn i und j genau 2 mal vorkommen
                        if (candidateOccurrences.at(i_1 - 1) == 2 && candidateOccurrences.at(i_2 - 1) == 2)
                        {
                            // Finde Felder, in denen i und j vorkommen
                            std::vector<Field*> hiddenPairFields;
#pragma unroll static_cast < short>(global::order)
                            for (Field* field : unit)
                            {
                                if (std::find(field->getCandidates()->begin(), field->getCandidates()->end(), i_1) != field->getCandidates()->end() &&
                                    std::find(field->getCandidates()->begin(), field->getCandidates()->end(), i_2) != field->getCandidates()->end())
                                {
                                    hiddenPairFields.push_back(field);
                                }
                            }
                            if (hiddenPairFields.size() == 2 && hiddenPairFields.at(0) != nullptr && hiddenPairFields.at(1) != nullptr &&
                                (hiddenPairFields.at(0)->getCandidates()->size() > 2 || hiddenPairFields.at(1)->getCandidates()->size() > 2))
                            {
                                const std::vector<uint8_t> cands({i_1, i_2}, std::allocator<uint8_t>());
                                return std::make_unique<HiddenSubset>(hiddenPairFields, cands, type).release();
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    void Sudoku::retainAll(std::vector<uint8_t>& candidates, const std::vector<uint8_t>& hiddenPairCandidates)
    {
        auto newEnd = std::remove_if(candidates.begin(), candidates.end(), [&hiddenPairCandidates](uint8_t value)
                                     { return std::find(hiddenPairCandidates.begin(), hiddenPairCandidates.end(), value) == hiddenPairCandidates.end(); });
        candidates.erase(newEnd, candidates.end());
    }

    void Sudoku::processHiddenPairs(const uint8_t run)
    {
#pragma unroll
        while (true)
        {
            HiddenSubset* firstHiddenPair = this->firstHiddenPair();
            if (firstHiddenPair == nullptr)
            {
                break;
            }
            const auto numCandsBeforeEliminatingHiddenPair = this->countCandidates();
            const std::vector<uint8_t> hiddenPairCandidates = firstHiddenPair->getCandidates();
            retainAll(*firstHiddenPair->getFields().at(0)->getCandidates(), hiddenPairCandidates);
            retainAll(*firstHiddenPair->getFields().at(1)->getCandidates(), hiddenPairCandidates);

            if (this->countCandidates() < numCandsBeforeEliminatingHiddenPair)
            {
                const std::string msg = "HiddenPair {" +
                                        std::to_string(firstHiddenPair->getCandidates().at(0)) + "," +
                                        std::to_string(firstHiddenPair->getCandidates().at(1)) + "} in " +
                                        firstHiddenPair->getType() + " " +
                                        std::to_string(firstHiddenPair->getUnitNumber()) + ": Fields (" +
                                        std::to_string(*firstHiddenPair->getFields().at(0)->getRID()) + "," +
                                        std::to_string(*firstHiddenPair->getFields().at(0)->getCID()) + ");(" +
                                        std::to_string(*firstHiddenPair->getFields().at(1)->getRID()) + "," +
                                        std::to_string(*firstHiddenPair->getFields().at(1)->getCID()) + ")";
                _steps.emplace_back(_grid, std::vector<Field*>{firstHiddenPair->getFields()}, std::vector<uint8_t>{firstHiddenPair->getCandidates()}, run, msg);
                _logTextArea->append(QString::fromStdString(msg));
            }
            // this->print();
            // this->printFields();
        }
    }

    // Naked Triple methods
    auto Sudoku::firstNakedTriple(std::vector<NakedTriple*>& deadNakedTriples) -> NakedTriple*
    {
        std::string row;
        row = "Row";
        std::string col;
        col = "Col";
        std::string block;
        block = "Block";
        const std::array<std::string, 3> types{row, col, block};
        for (const std::string& type : types)
        {
            for (uint8_t unitID = 1; unitID <= global::order; unitID++) // go over all units
            {
                auto unit = this->getUnit(type, unitID);
                uint8_t numFreeFieldsInUnit = 0; // In der jeweiligen Unit muessen > 3 freie Felder sein
#pragma unroll static_cast < short>(global::order)
                for (uint8_t fID = 1; fID <= global::order; fID++)
                {
                    if (*unit.at(fID - 1)->getVal() == 0)
                    {
                        numFreeFieldsInUnit++;
                    }
                }

                for (uint8_t i_1 = 1; i_1 <= global::order - 2; i_1++) // Alle Zahlen-Tripel {1,2,3} bis {7,8,9}
                {
                    for (uint8_t i_2 = i_1 + 1; i_2 <= global::order - 1; i_2++)
                    {
                        for (uint8_t i_3 = i_2 + 1; i_3 <= global::order; i_3++)
                        {
                            std::vector<uint8_t> nonMatchCands;
                            // Kandidatenlisten aller drei Felder duerfen ausschliesslich die Zahlen i,j,k enthalten
#pragma unroll static_cast < short>(global::order)
                            for (uint8_t cand = 1; cand <= global::order; cand++)
                            {
                                if (cand != i_1 && cand != i_2 && cand != i_3)
                                {
                                    nonMatchCands.push_back(cand);
                                }
                            }
                            for (uint8_t fID1 = 1; fID1 <= global::order - 2; fID1++) // Alle Feld-Kombinationen in unit
                            {
                                for (uint8_t fID2 = fID1 + 1; fID2 <= global::order - 1; fID2++)
                                {
                                    for (uint8_t fID3 = fID2 + 1; fID3 <= global::order; fID3++)
                                    {
                                        Field* field1 = unit.at(fID1 - 1);
                                        Field* field2 = unit.at(fID2 - 1);
                                        Field* field3 = unit.at(fID3 - 1);
                                        if (*field1->getVal() == 0 && *field2->getVal() == 0 && *field3->getVal() == 0)
                                        {
                                            bool flIsNakedTriple = true;
#pragma unroll static_cast < short>(global::order)
                                            for (const uint8_t cand : nonMatchCands)
                                            {
                                                if (std::find(field1->getCandidates()->begin(), field1->getCandidates()->end(), cand) != field1->getCandidates()->end())
                                                {
                                                    flIsNakedTriple = false;
                                                }
                                            }
                                            bool fmIsNakedTriple = true;
#pragma unroll static_cast < short>(global::order)
                                            for (const uint8_t cand : nonMatchCands)
                                            {
                                                if (std::find(field2->getCandidates()->begin(), field2->getCandidates()->end(), cand) != field2->getCandidates()->end())
                                                {
                                                    fmIsNakedTriple = false;
                                                }
                                            }
                                            bool fnIsNakedTriple = true;
#pragma unroll static_cast < short>(global::order)
                                            for (const uint8_t cand : nonMatchCands)
                                            {
                                                if (std::find(field3->getCandidates()->begin(), field3->getCandidates()->end(), cand) != field3->getCandidates()->end())
                                                {
                                                    fnIsNakedTriple = false;
                                                }
                                            }
                                            if (flIsNakedTriple && fmIsNakedTriple && fnIsNakedTriple && numFreeFieldsInUnit > 3)
                                            {
                                                auto* firstNakedTriple = std::make_unique<NakedTriple>(field1, field2, field3, i_1, i_2, i_3, type).release();
                                                bool isDead = false;
#pragma unroll 100
                                                for (NakedTriple* dead : deadNakedTriples)
                                                {
                                                    if (dead->getField1() == firstNakedTriple->getField1() &&
                                                        dead->getField2() == firstNakedTriple->getField2() &&
                                                        dead->getField3() == firstNakedTriple->getField3() &&
                                                        dead->getType() == firstNakedTriple->getType())
                                                    {
                                                        isDead = true;
                                                    }
                                                }
                                                if (!isDead)
                                                {
                                                    deadNakedTriples.push_back(firstNakedTriple);
                                                    return firstNakedTriple;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    void Sudoku::eliminateCandidatesOfNakedTripleInUnit(NakedTriple* nakedTriple, const std::array<Field*, global::order>& unit)
    {
        for (Field* field : unit)
        {
            if (field->getFID() == nakedTriple->getField1()->getFID() ||
                field->getFID() == nakedTriple->getField2()->getFID() ||
                field->getFID() == nakedTriple->getField3()->getFID() ||
                *field->getVal() != 0)
            {
                continue;
            }
            std::vector<uint8_t>* candidates = field->getCandidates();
            const std::vector<uint8_t> candidatesToRemove({nakedTriple->getCandidate1(), nakedTriple->getCandidate2(), nakedTriple->getCandidate3()}, std::allocator<uint8_t>());
#pragma unroll static_cast < short>(global::order)
            for (const uint8_t candidate : candidatesToRemove)
            {
                candidates->erase(std::remove(candidates->begin(), candidates->end(), candidate), candidates->end());
            }
        }
    }

    void Sudoku::eliminateNakedTriple(NakedTriple* nakedTriple)
    {
        if (*nakedTriple->getField1()->getRID() == *nakedTriple->getField2()->getRID() && *nakedTriple->getField1()->getRID() == *nakedTriple->getField3()->getRID() && *nakedTriple->getField2()->getRID() == *nakedTriple->getField3()->getRID())
        {
            Sudoku::eliminateCandidatesOfNakedTripleInUnit(nakedTriple, this->getRowByFieldID(*nakedTriple->getField1()->getFID()));
        }
        if (*nakedTriple->getField1()->getCID() == *nakedTriple->getField2()->getCID() && *nakedTriple->getField1()->getCID() == *nakedTriple->getField3()->getCID() && *nakedTriple->getField2()->getCID() == *nakedTriple->getField3()->getCID())
        {
            Sudoku::eliminateCandidatesOfNakedTripleInUnit(nakedTriple, this->getColByFieldID(*nakedTriple->getField1()->getFID()));
        }
        if (*nakedTriple->getField1()->getBID() == *nakedTriple->getField2()->getBID() && *nakedTriple->getField1()->getBID() == *nakedTriple->getField3()->getBID() && *nakedTriple->getField2()->getBID() == *nakedTriple->getField3()->getBID())
        {
            Sudoku::eliminateCandidatesOfNakedTripleInUnit(nakedTriple, this->getBlockByFieldID(*nakedTriple->getField1()->getFID()));
        }
    }

    void Sudoku::processNakedTriples(const uint8_t run)
    {
        std::vector<NakedTriple*> deadNakedTriples;
#pragma unroll
        while (true)
        {
            NakedTriple* firstNakedTriple = this->firstNakedTriple(deadNakedTriples);
            if (firstNakedTriple == nullptr)
            {
                break;
            }
            const auto numCandsBeforeEliminatingFirstNakedTriple = this->countCandidates();
            this->eliminateNakedTriple(firstNakedTriple);
            if (this->countCandidates() < numCandsBeforeEliminatingFirstNakedTriple)
            {
                const std::string msg =
                    "NakedTriple {" +
                    std::to_string(firstNakedTriple->getCandidate1()) + "," +
                    std::to_string(firstNakedTriple->getCandidate2()) + "," +
                    std::to_string(firstNakedTriple->getCandidate3()) + "} in " +
                    firstNakedTriple->getType() + " " +
                    std::to_string(firstNakedTriple->getUnitNumber()) + ": Fields (" +
                    std::to_string(*firstNakedTriple->getField1()->getRID()) + "," +
                    std::to_string(*firstNakedTriple->getField1()->getCID()) + ");(" +
                    std::to_string(*firstNakedTriple->getField2()->getRID()) + "," +
                    std::to_string(*firstNakedTriple->getField2()->getCID()) + ");(" +
                    std::to_string(*firstNakedTriple->getField3()->getRID()) + "," +
                    std::to_string(*firstNakedTriple->getField3()->getCID()) + ")";
                _steps.emplace_back(_grid, std::vector<Field*>({firstNakedTriple->getField1(), firstNakedTriple->getField2(), firstNakedTriple->getField3()}, std::allocator<Field*>()), std::vector<uint8_t>({firstNakedTriple->getCandidate1(), firstNakedTriple->getCandidate2(), firstNakedTriple->getCandidate3()}, std::allocator<uint8_t>()), run, msg);
                _logTextArea->append(QString::fromStdString(msg));
            }
            if (this->firstNakedSingle() != nullptr || this->firstHiddenSingle() != nullptr)
            {
                return;
            }
        }
        // this->print();
        // this->printFields();
    }

    // Hidden Triple methods
    auto Sudoku::firstHiddenTriple() -> HiddenSubset*
    {
        std::string row;
        row = "Row";
        std::string col;
        col = "Col";
        std::string block;
        block = "Block";
        const std::array<std::string, 3> types{row, col, block};
        for (const std::string& type : types)
        {
            for (uint8_t unitID = 1; unitID <= global::order; unitID++) // go over all units
            {
                auto unit = this->getUnit(type, unitID);
                std::array<uint8_t, global::order> candidateOccurrences = candidateOccurrencesInUnit(unit);
                for (uint8_t i_1 = 1; i_1 <= global::order - 2; i_1++) // Alle Zahlen-Tripel {1,2,3} bis {7,8,9}
                {
                    for (uint8_t i_2 = i_1 + 1; i_2 <= global::order - 1; i_2++)
                    {
                        for (uint8_t i_3 = i_2 + 1; i_3 <= global::order; i_3++)
                        {
                            if (candidateOccurrences.at(i_1 - 1) > 0 && candidateOccurrences.at(i_2 - 1) > 0 && candidateOccurrences.at(i_3 - 1) > 0)
                            {
                                for (uint8_t fID1 = 1; fID1 <= global::order - 2; fID1++) // Alle Feld-Kombinationen in unit
                                {
                                    for (uint8_t fID2 = fID1 + 1; fID2 <= global::order - 1; fID2++)
                                    {
                                        for (uint8_t fID3 = fID2 + 1; fID3 <= global::order; fID3++)
                                        {
                                            // i,j,k are only allowed in fields l,m,n
                                            Field* field1 = unit.at(fID1 - 1);
                                            Field* field2 = unit.at(fID2 - 1);
                                            Field* field3 = unit.at(fID3 - 1);

                                            if ((*field1->getVal() != 0 || *field2->getVal() != 0 || *field3->getVal() != 0) ||
                                                (field1->getCandidates()->size() <= 3 && field2->getCandidates()->size() <= 3 && field3->getCandidates()->size() <= 3))
                                            {
                                                continue;
                                            }
                                            bool isHiddenTriple = true;
#pragma unroll static_cast<short>(global::order)
                                            for (Field* field : unit)
                                            {
                                                if (field != field1 && field != field2 && field != field3)
                                                {
                                                    if (std::find(field->getCandidates()->begin(), field->getCandidates()->end(), i_1) != field->getCandidates()->end() ||
                                                        std::find(field->getCandidates()->begin(), field->getCandidates()->end(), i_2) != field->getCandidates()->end() ||
                                                        std::find(field->getCandidates()->begin(), field->getCandidates()->end(), i_3) != field->getCandidates()->end())
                                                    {
                                                        isHiddenTriple = false;
                                                    }
                                                }
                                            }
                                            if (isHiddenTriple)
                                            {
                                                return std::make_unique<HiddenSubset>(std::vector<Field*>({field1, field2, field3}, std::allocator<Field*>()), std::vector<uint8_t>({i_1, i_2, i_3}, std::allocator<uint8_t>()), type).release();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    void Sudoku::processHiddenTriples(const uint8_t run)
    {
        while (true)
        {
            HiddenSubset* firstHiddenTriple = this->firstHiddenTriple();
            if (firstHiddenTriple == nullptr)
            {
                break;
            }
            const auto numCandsBeforeEliminatingHiddenTriple = this->countCandidates();
            const std::vector<uint8_t> hiddenTripleCanidates = firstHiddenTriple->getCandidates();
#pragma unroll 3
            for (Field* field : firstHiddenTriple->getFields())
            {
                retainAll(*field->getCandidates(), hiddenTripleCanidates);
            }
            if (this->countCandidates() < numCandsBeforeEliminatingHiddenTriple)
            {
                const std::string msg = "HiddenTriple {" +
                                        std::to_string(firstHiddenTriple->getCandidates().at(0)) + "," +
                                        std::to_string(firstHiddenTriple->getCandidates().at(1)) + "," +
                                        std::to_string(firstHiddenTriple->getCandidates().at(2)) + "} in " +
                                        firstHiddenTriple->getType() + " " +
                                        std::to_string(firstHiddenTriple->getUnitNumber()) + ": Fields (" +
                                        std::to_string(*firstHiddenTriple->getFields().at(0)->getRID()) + "," +
                                        std::to_string(*firstHiddenTriple->getFields().at(0)->getCID()) + ");(" +
                                        std::to_string(*firstHiddenTriple->getFields().at(1)->getRID()) + "," +
                                        std::to_string(*firstHiddenTriple->getFields().at(1)->getCID()) + ");(" +
                                        std::to_string(*firstHiddenTriple->getFields().at(2)->getRID()) + "," +
                                        std::to_string(*firstHiddenTriple->getFields().at(2)->getCID()) + ")";
                _steps.emplace_back(_grid, std::vector<Field*>{firstHiddenTriple->getFields()}, std::vector<uint8_t>{firstHiddenTriple->getCandidates()}, run, msg);
                _logTextArea->append(QString::fromStdString(msg));
            }
            if (this->firstNakedSingle() != nullptr || this->firstHiddenSingle() != nullptr)
            {
                return;
            }
        }
        // this->print();
        // this->printFields();
    }

    // Intersections / Locked Candidates
    // Sub-methods for RBC and BRC
    auto Sudoku::findFieldsInUnitContainingCandidateI(const std::array<Field*, global::order>& unit, const uint8_t cand) -> std::vector<Field*>
    {
        std::vector<Field*> containingCandidateI{};
#pragma unroll static_cast<short>(global::order)
        for (Field* field : unit) // collect all Fields containing candidate i
        {
            if (std::find(field->getCandidates()->begin(), field->getCandidates()->end(), cand) != field->getCandidates()->end())
            {
                containingCandidateI.push_back(field);
            }
        }
        return containingCandidateI;
    }

    void Sudoku::removeCandidateIFromUnit(const uint8_t cand, const std::vector<Field*>& containingCandidateI, const uint8_t unitIdOfFirst, const std::string& type)
    {
        auto unit = this->getUnit(type, unitIdOfFirst);
#pragma unroll static_cast<short>(global::order)
        for (Field* field : unit)
        {
            if (std::find(field->getCandidates()->begin(), field->getCandidates()->end(), cand) != field->getCandidates()->end() &&
                !(std::find(containingCandidateI.begin(), containingCandidateI.end(), field) != containingCandidateI.end()))
            {
                field->getCandidates()->erase(std::remove(field->getCandidates()->begin(), field->getCandidates()->end(), cand), field->getCandidates()->end());
            }
        }
    }

    void Sudoku::eliminateBRCfromLine(const uint8_t run, const uint8_t blockID, const uint8_t cand, const std::vector<Field*>& containingCandidateI, const std::string& type, const uint8_t lineIdOfFirst, const bool allCandidatesInSameLine)
    {
        if (allCandidatesInSameLine) // eliminate all other candidtes in the block
        {
            const auto numCandsBeforeBRC = this->countCandidates();
            this->removeCandidateIFromUnit(cand, containingCandidateI, lineIdOfFirst, type);
            if (this->countCandidates() < numCandsBeforeBRC)
            {
                const std::string msg = "Block-in-" +
                                        type + " with {" +
                                        std::to_string(cand) + "} in Block " +
                                        std::to_string(blockID) + ": Only in " +
                                        type + " " + std::to_string(lineIdOfFirst);
                _steps.emplace_back(_grid, containingCandidateI, std::vector<uint8_t>({cand}, std::allocator<uint8_t>()), run, msg);
                _logTextArea->append(QString::fromStdString(msg));
            }
        }
    }

    // Block-Row-Checks (Pointing)
    void Sudoku::performBlockRowChecks(const uint8_t run)
    {
        for (uint8_t blockID = 1; blockID <= global::order; blockID++)
        {
            const std::array<Field*, global::order> block = this->getBlockByBlockID(blockID);
            for (uint8_t cand = 1; cand <= global::order; cand++)
            {
                std::vector<Field*> containingCandidateI = Sudoku::findFieldsInUnitContainingCandidateI(block, cand);
                if (!containingCandidateI.empty())
                {
                    std::string row;
                    row = "Row";
                    std::string col;
                    col = "Col";
                    const std::array<std::string, 2> types{row, col};
                    for (const std::string& type : types)
                    {
                        if (type == "Row")
                        {
                            const uint8_t rowIdOfFirst = *containingCandidateI.at(0)->getRID();
                            bool allCandidatesInSameRow = false;
#pragma unroll static_cast<short>(global::order)
                            for (Field* field : containingCandidateI)
                            {
                                if (*field->getRID() == rowIdOfFirst)
                                {
                                    allCandidatesInSameRow = true;
                                }
                                else
                                {
                                    allCandidatesInSameRow = false;
                                    break;
                                }
                            }
                            this->eliminateBRCfromLine(run, blockID, cand, containingCandidateI, type, rowIdOfFirst, allCandidatesInSameRow);
                        }
                        else if (type == "Col")
                        {
                            const uint8_t colIdOfFirst = *containingCandidateI.at(0)->getCID();
                            bool allCandidatesInSameCol = false;
#pragma unroll static_cast<short>(global::order)
                            for (Field* field : containingCandidateI)
                            {
                                if (*field->getCID() == colIdOfFirst)
                                {
                                    allCandidatesInSameCol = true;
                                }
                                else
                                {
                                    allCandidatesInSameCol = false;
                                    break;
                                }
                            }
                            this->eliminateBRCfromLine(run, blockID, cand, containingCandidateI, type, colIdOfFirst, allCandidatesInSameCol);
                        }
                    }
                }
            }
        }
        // this->print();
        // this->printFields();
    }

    // Row-Block-Checks (Claiming)
    void Sudoku::performRowBlockChecks(const uint8_t run)
    {
        std::string row;
        row = "Row";
        std::string col;
        col = "Col";
        const std::array<std::string, 2> types{row, col};
        for (const std::string& type : types)
        {
            for (uint8_t lineID = 1; lineID <= global::order; lineID++) // Iterate over each row/col
            {
                auto unit = this->getUnit(type, lineID);
                for (uint8_t cand = 1; cand <= global::order; cand++)
                {
                    std::vector<Field*> containingCandidateI = Sudoku::findFieldsInUnitContainingCandidateI(unit, cand);
                    if (!containingCandidateI.empty()) // proceed only if there exists at least one field with candidate cand
                    {
                        const uint8_t blockIdOfFirst = *containingCandidateI.at(0)->getBID();
                        bool allCandidatesInSameBlock = false;
#pragma unroll static_cast<short>(global::order)
                        for (Field* field : containingCandidateI)
                        {
                            if (*field->getBID() == blockIdOfFirst)
                            {
                                allCandidatesInSameBlock = true;
                            }
                            else
                            {
                                allCandidatesInSameBlock = false;
                                break;
                            }
                        }
                        if (allCandidatesInSameBlock)
                        {
                            const auto numCandsBeforeRBC = this->countCandidates();
                            std::string blockStr;
                            blockStr = "Block";
                            this->removeCandidateIFromUnit(cand, containingCandidateI, blockIdOfFirst, blockStr); // remove candidate cand from blockStr
                            if (this->countCandidates() < numCandsBeforeRBC)
                            {
                                std::string msg = type;
                                msg.append("-in-Block with {" +
                                           std::to_string(cand) + "} in " +
                                           type + " " +
                                           std::to_string(lineID) + ": Only in Block " +
                                           std::to_string(blockIdOfFirst));
                                _steps.emplace_back(_grid, containingCandidateI, std::vector<uint8_t>({cand}, std::allocator<uint8_t>()), run, msg);
                                _logTextArea->append(QString::fromStdString(msg));
                            }
                        }
                    }
                }
            }
        }
        // this->print();
        // this->printFields();
    }

    // Last resort: Try and error with backtracking

    auto Sudoku::unitContainsVal(const uint8_t val, const std::array<Field*, global::order>& unit) -> bool
    {
#pragma unroll static_cast<short>(global::order)
        for (auto* field : unit)
        {
            if (*field->getVal() == val)
            {
                return true;
            }
        }
        return false;
    }

    auto Sudoku::backtracking() -> bool
    {
        auto freeFields = this->getFreeFields();
        if (freeFields.empty()) // Abort recursion: No free fields remaining
        {
            return true;
        }
        auto* firstFreeField = freeFields.front();
#pragma unroll static_cast<short>(global::order)
        for (uint8_t val = 1; val <= global::order; val++)
        {
            if (!Sudoku::unitContainsVal(val, this->getRowByFieldID(*firstFreeField->getFID())) &&
                !Sudoku::unitContainsVal(val, this->getColByFieldID(*firstFreeField->getFID())) &&
                !Sudoku::unitContainsVal(val, this->getBlockByFieldID(*firstFreeField->getFID())))
            {
                firstFreeField->setCandidates(std::vector<uint8_t>({val}, std::allocator<uint8_t>()));
                this->filldAndEliminate(firstFreeField);
                if (this->backtracking())
                {
                    return true;
                }
                firstFreeField->setVal(0);
            }
        }
        return false;
    }

    // Main Solving routine
    void Sudoku::solve()
    {
        _logTextArea->clear();
        _logTextArea->append("START SOLVING SUDOKU '" + QString::fromStdString(_name) + "'");
        _logTextArea->append("Initial free fields: " + QString::number(this->getFreeFields().size(), global::base));
        _logTextArea->append("Initial candidates: " + QString::number(this->countCandidates(), global::base) + "\n");

        uint8_t run = 0;

        _steps.emplace_back(_grid, std::vector<Field*>{}, std::vector<uint8_t>{}, run, "Initial status");

        std::chrono::high_resolution_clock::time_point t_1;
        const std::chrono::high_resolution_clock::time_point t_0 = std::chrono::high_resolution_clock::now();

        // Main solving loop
#pragma unroll
        while (true)
        {
            run++;

            const auto numCandsBeforeRun = this->countCandidates();

            _logTextArea->append("BEGIN Run No. " + QString::number(run, global::base) + ":\n");

            // Process Singles
            if (_useNakedSingles)
            {
                _logTextArea->append(QStringLiteral("Search for NakedSingles"));
                this->processNakedSingles(run); // Check Naked Singles
            }
            else
            {
                _logTextArea->append(QStringLiteral("Skip NakedSingles"));
            }
            if (_useHiddenSingles)
            {
                _logTextArea->append(QStringLiteral("\nSearch for HiddenSingles"));
                this->processHiddenSingles(run); // Check Hidden Singles
            }
            else
            {
                _logTextArea->append(QStringLiteral("\nSkip HiddenSingles"));
            }

            const auto numCandsAfterSingles = this->countCandidates();

            // If no (more) Singles exist -> Search for Naked and Hidden Pairs
            if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && this->countCandidates() > 0)
            {
                if (numCandsAfterSingles < numCandsBeforeRun)
                {
                    _logTextArea->append(QStringLiteral("\nNo further Singles found.\nProceeding with Pairs..."));
                }
                else
                {
                    _logTextArea->append(QStringLiteral("\nNo Singles found.\nProceeding with pairs..."));
                }

                const auto numCandsBeforePairs = this->countCandidates();
                if (_useNakedPairs)
                {
                    _logTextArea->append(QStringLiteral("\nSearch for NakedPairs"));
                    this->processNakedPairs(run); // Check Naked Pairs
                }
                else
                {
                    _logTextArea->append(QStringLiteral("\nSkip NakedPairs"));
                }
                if (_useHiddenPairs)
                {
                    _logTextArea->append(QStringLiteral("\nSearch for HiddenPairs"));
                    this->processHiddenPairs(run); // Check Hidden Pairs
                }
                else
                {
                    _logTextArea->append(QStringLiteral("\nSkip HiddenPairs"));
                }
                const auto numCandsAfterPairs = this->countCandidates();

                // If Pair techniques didn't produce Singles -> search for Naked and Hidden Triples
                if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && !this->getFreeFields().empty())
                {
                    if (numCandsAfterPairs < numCandsBeforePairs)
                    {
                        _logTextArea->append(QStringLiteral("\nPair techniques didn't produce Singles.\nProceeding with Triples..."));
                    }
                    else
                    {
                        _logTextArea->append(QStringLiteral("\nNo Pairs found.\nProceeding with Triples..."));
                    }

                    const auto numCandsBeforeTriples = this->countCandidates();
                    if (_useNakedTriples)
                    {
                        _logTextArea->append(QStringLiteral("\nSearch for NakedTriples"));
                        this->processNakedTriples(run); // Check Naked Triples
                    }
                    else
                    {
                        _logTextArea->append(QStringLiteral("\nSkip NakedTriples"));
                    }
                    if (_useHiddenTriples)
                    {
                        _logTextArea->append(QStringLiteral("\nSearch for HiddenTriples"));
                        this->processHiddenTriples(run); // Check Hidden Triples
                    }
                    else
                    {
                        _logTextArea->append(QStringLiteral("\nSkip HiddenTriples"));
                    }
                    const auto numCandsAfterTriples = this->countCandidates();

                    // If pair/triple techniques didn't produce Singles -> go for Row-Block-Checks and Block-Row-Checks
                    if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && !this->getFreeFields().empty())
                    {
                        if (numCandsAfterTriples < numCandsBeforeTriples)
                        {
                            _logTextArea->append(QStringLiteral("\nTriple techniques didn't produce Singles.\nProceeding with Locked Candidates..."));
                        }
                        else
                        {
                            _logTextArea->append(QStringLiteral("\nNo Triples found.\nProceeding with Locked Candidates..."));
                        }

                        const auto numCandsBeforeLockedCands = this->countCandidates();
                        if (_useBlockLineChecks)
                        {
                            _logTextArea->append(QStringLiteral("\nPerform Block-Line-Interactions"));
                            this->performBlockRowChecks(run); // Perform Block-Row-Checks (Pointing)
                        }
                        else
                        {
                            _logTextArea->append(QStringLiteral("\nSkip Block-Line-Interactions"));
                        }
                        if (_useLineBlockChecks)
                        {
                            _logTextArea->append(QStringLiteral("\nPerform Line-Block-Interactions"));
                            this->performRowBlockChecks(run); // Perform Row-Block-Checks (Claiming)
                        }
                        else
                        {
                            _logTextArea->append(QStringLiteral("\nSkip Line-Block-Interactions"));
                        }
                        const auto numCandsAfterLockedCands = this->countCandidates();

                        // If pair/triple/LockedCands didn't produce Singles -> go for further technique
                        if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && !this->getFreeFields().empty())
                        {
                            if (numCandsAfterLockedCands < numCandsBeforeLockedCands)
                            {
                                _logTextArea->append(QStringLiteral("\nLockedCandidates didn't produce Singles.\nNeed further techniques..."));
                            }
                            else
                            {
                                _logTextArea->append(QStringLiteral("\nNo LockedCandidates found.\nNeed further techniques..."));
                            }

                            const auto numCandsBeforeFurtherTechnique = this->countCandidates();
                            // to do implement further techniques
                            const auto numCandsAfterFurtherTechnique = this->countCandidates();

                            // If further technique didn't produce Singles -> go for another further technique
                            if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && !this->getFreeFields().empty())
                            {
                                if (numCandsAfterFurtherTechnique < numCandsBeforeFurtherTechnique)
                                {
                                    _logTextArea->append(QStringLiteral("\nFurther technique didn't produce Singles.\nNo further techniques implemented..."));
                                }
                                else
                                {
                                    _logTextArea->append(QStringLiteral("\nNo more further technique.\nNo further techniques implemented..."));
                                }
                            }
                            else
                            {
                                _logTextArea->append(QStringLiteral("\nFurther technique produced Singles.\nProceeding with next run..."));
                            }
                        }
                        else
                        {
                            _logTextArea->append(QStringLiteral("\nLockedCandidates produced Singles.\nProceeding with next run..."));
                        }
                    }
                    else
                    {
                        _logTextArea->append(QStringLiteral("\nTriple techniques produced Singles.\nProceeding with next run..."));
                    }
                }
                else
                {
                    _logTextArea->append(QStringLiteral("\nPair techniques produced Singles.\nProceeding with next run..."));
                }
            }
            else
            {
                if (!this->getFreeFields().empty() && this->firstNakedSingle() != nullptr && _useNakedSingles)
                {
                    _logTextArea->append(QStringLiteral("\nNo more HiddenSingles, but new NakedSingles.\nProceeding with next run..."));
                }
            }

            const auto freeFieldsAfterRun = this->getFreeFields().size();
            const auto numCandsAfterRun = this->countCandidates();

            _logTextArea->append("\nEND Run No. " + QString::number(run, global::base) + ":");
            _logTextArea->append(QString::number(freeFieldsAfterRun, global::base) + " free fields, " + QString::number(numCandsAfterRun, global::base) + " candidates\n‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒\n");

            if (numCandsAfterRun == 0)
            {
                t_1 = std::chrono::high_resolution_clock::now();
                _logTextArea->append(QStringLiteral("Finished: No free fields remaining\n"));
                break;
            }

            if (numCandsBeforeRun == numCandsAfterRun)
            {
                _logTextArea->append(QStringLiteral("Abort: No more solving techniques implemented\n"));
                if (_useBacktracking)
                {
                    this->backtracking();
                    t_1 = std::chrono::high_resolution_clock::now();
                    _logTextArea->append(QStringLiteral("Last resort: Proceed with try-and-error (backtracking)\n"));
                    _steps.emplace_back(_grid, std::vector<Field*>{}, std::vector<uint8_t>{}, run, "Solution after try-and-error (backtracking)");
                }
                else
                {
                    t_1 = std::chrono::high_resolution_clock::now();
                }
                break;
            }
        }

        if (this->getFreeFields().empty())
        {
            _logTextArea->append("SUDOKU '" + QString::fromStdString(_name) + "' SUCESSFULLY SOLVED!\n");
        }
        else
        {
            _logTextArea->append("SOLVING SUDOKU '" + QString::fromStdString(_name) + "' FAILED!\n");
            _logTextArea->append(QStringLiteral("\nAbort status:"));
            this->print();
            this->printFields();
            _logTextArea->append("\nAbort with " + QString::number(this->getFreeFields().size(), global::base) + " free fields and " + QString::number(this->countCandidates(), global::base) + " candidates\n");
        }

        auto duration = std::chrono::duration<double, std::milli>(t_1 - t_0).count();
        constexpr float sec = 1000.F;
        constexpr float min = 60.F;
        constexpr uint8_t minInt = 60;
        if (duration < sec)
        {
            _logTextArea->append("Elapsed time: " + QString::number(std::chrono::duration<double, std::milli>(t_1 - t_0).count(), 'f', 1) + " ms\n");
        }
        else
        {
            duration /= sec;
            if (duration < min)
            {
                _logTextArea->append("Elapsed time: " + QString::number(duration, 'f', 1) + " s\n");
            }
            else
            {
                auto sec = static_cast<uint8_t>(duration) % minInt;
                duration /= min;
                _logTextArea->append("Elapsed time: " + QString::number(duration, 'f', 0) + ":" + QString::number(sec, global::base) + " min\n");
            }
        }
        _logTextArea->repaint();
    }

    void Sudoku::print() const
    {
        QString printString;
        QTextStream printStream(&printString, QIODevice::ReadWrite);

        printStream << "col  1  2  3   4  5  6   7  8  9  " << Qt::endl;
        printStream << "row ----------------------------- " << Qt::endl;
        uint8_t rowID = 1;
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
            QString str = " " + QString::number(rowID, global::base) + " |";
            uint8_t colID = 1;
#pragma unroll static_cast<short>(global::order)
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                const uint8_t val = *_grid.at(rID - 1).at(cID - 1).getVal();
                if (val != 0)
                {
                    str += " " + QString::number(val, global::base) + " ";
                }
                else
                {
                    str += QStringLiteral("   ");
                }
                if (colID % 3 == 0 && colID != global::order)
                {
                    str += QStringLiteral("|");
                }
                colID++;
            }
            printStream << str << "|" << Qt::endl;
            if (rowID % 3 == 0 && rowID != global::order)
            {
                printStream << "   |---------+---------+---------|" << Qt::endl;
            }
            rowID++;
        }
        printStream << "    ----------------------------- " << Qt::endl;
        printStream << Qt::endl;

        _logTextArea->append(printString);
    }

    void Sudoku::printFields() const
    {
        QString printString;
        QTextStream printStream(&printString, QIODevice::ReadWrite);
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
#pragma unroll static_cast<short>(global::order)
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                _grid.at(rID - 1).at(cID - 1).printField(*_logTextArea);
            }
        }
        printStream << Qt::endl;
        _logTextArea->append(printString);
    }

    auto Sudoku::getUnit(const std::string& type, uint8_t unitID) -> std::array<Field*, global::order>
    {
        if (type == "Row")
        {
            return this->getRowByRowID(unitID);
        }
        if (type == "Col")
        {
            return this->getColByColID(unitID);
        }
        if (type == "Block")
        {
            return this->getBlockByBlockID(unitID);
        }
        return std::array<Field*, global::order>{};
    }

    auto Sudoku::getUnitNumber(Field* field, const std::string& type) -> uint8_t
    {
        if (type == "Row")
        {
            return *field->getRID();
        }
        if (type == "Col")
        {
            return *field->getCID();
        }
        if (type == "Block")
        {
            return *field->getBID();
        }
        return 0;
    }
} // namespace sudoku
