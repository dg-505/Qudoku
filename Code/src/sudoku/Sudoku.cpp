#include "sudoku/Sudoku.h"
#include <QtCore/QString>
#include <QtCore/QTextStream>

namespace sudoku
{
    Sudoku::Sudoku(const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>* vals, QLogTextBrowser& logTextArea)
        : _logTextArea(&logTextArea)
    {
        // Fill values
        uint8_t fID = 1;
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
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
                    for (uint8_t cand = 1; cand <= global::order; cand++)
                    {
                        candidates.push_back(cand);
                    }
                    field->setCandidates(&candidates);
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
                for (uint8_t rID = 1; rID <= global::order; rID++)
                {
                    const uint8_t* val = row.at(rID - 1)->getVal();
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
                for (uint8_t cID = 1; cID <= global::order; cID++)
                {
                    const uint8_t* val = col.at(cID - 1)->getVal();
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

    auto Sudoku::getSteps() -> std::vector<std::array<std::array<Field, global::order>, global::order>>*
    {
        return &_steps;
    }

    auto Sudoku::getLogTextArea() -> QLogTextBrowser*
    {
        return _logTextArea;
    }

    auto Sudoku::getFoundInRunNo() -> std::vector<uint8_t>*
    {
        return &_foundInRunNo;
    }

    auto Sudoku::getFoundByType() -> std::vector<QString>*
    {
        return &_foundByType;
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

    auto Sudoku::getRowByRowID(const uint8_t rID) -> std::array<Field*, global::order>
    {
        std::array<Field*, global::order> row{};
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

    auto Sudoku::countCandidates() -> uint8_t
    {
        uint8_t numCands = 0;
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
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
            const QString errorMessage = "Error filling Field (" + QString::number(*field->getRID()) + "," + QString::number(*field->getCID()) + "): Field has more than one candidate!";
            this->_logTextArea->append(errorMessage);
            return;
        }
        const uint8_t val = (*field->getCandidates())[0];
        field->setVal(&val);

        // eliminate candidate "val" from units
        auto rowFields = this->getRowByFieldID(*field->getFID());
        for (Field* field : rowFields)
        {
            field->getCandidates()->erase(std::remove(field->getCandidates()->begin(), field->getCandidates()->end(), val), field->getCandidates()->end());
        }

        auto colFields = this->getColByFieldID(*field->getFID());
        for (Field* field : colFields)
        {
            field->getCandidates()->erase(std::remove(field->getCandidates()->begin(), field->getCandidates()->end(), val), field->getCandidates()->end());
        }

        auto blockFields = this->getBlockByFieldID(*field->getFID());
        for (Field* field : blockFields)
        {
            field->getCandidates()->erase(std::remove(field->getCandidates()->begin(), field->getCandidates()->end(), val), field->getCandidates()->end());
        }

        auto freeFields = this->getFreeFields();
        freeFields.erase(std::remove(freeFields.begin(), freeFields.end(), field), freeFields.end());
    }

    // Naked Single methods
    auto Sudoku::firstNakedSingle() -> Field*
    {
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
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
        this->_logTextArea->append(QStringLiteral("Search for NakedSingles"));
        while (true)
        {
            Field* firstNakedSingle = this->firstNakedSingle();
            if (firstNakedSingle == nullptr)
            {
                break;
            }
            this->filldAndEliminate(firstNakedSingle);
            const QString msg =
                "NakedSingle {" + QString::number(*firstNakedSingle->getVal()) +
                "} in Field (" + QString::number(*firstNakedSingle->getRID()) + "," +
                QString::number(*firstNakedSingle->getCID()) + ")";
            this->_logTextArea->append(msg);
            this->addStepToList(run, msg);
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
        // Reihenfolge: Erst Reihen, dann Spalten, dann Bloecke
        // nach HiddenSingle durchsuchen
        const std::array<std::string, 3> types{"Row", "Col", "Block"};
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
                        for (uint8_t fID = 1; fID <= global::order; fID++)
                        {
                            Field* field = unit.at(fID - 1);
                            if (fieldContainsCandidate(field->getCandidates(), i))
                            {
                                // Kandidat fuer Feld f ist nur Zahl i
                                return new HiddenSingle(field, i, type);
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
        this->_logTextArea->append(QStringLiteral("\nSearch for HiddenSingles"));
        while (true)
        {
            HiddenSingle* firstHiddenSingle = this->firstHiddenSingle();
            if (firstHiddenSingle == nullptr)
            {
                break;
            }
            const std::vector<uint8_t> cand(1, *firstHiddenSingle->getCandidate());
            firstHiddenSingle->getField()->setCandidates(&cand);
            this->filldAndEliminate(firstHiddenSingle->getField());

            const QString msg =
                "HiddenSingle {" +
                QString::number(*firstHiddenSingle->getField()->getVal()) + "} in " +
                QString::fromStdString(*firstHiddenSingle->getType()) + " " +
                QString::number(*firstHiddenSingle->getUnitNumber()) + ": Field (" +
                QString::number(*firstHiddenSingle->getField()->getRID()) + "," +
                QString::number(*firstHiddenSingle->getField()->getCID()) + ")";

            this->addStepToList(run, msg);
            this->_logTextArea->append(msg);
        }
        // this->print();
        // this->printFields();
    }

    // Naked Pair methods
    auto Sudoku::firstNakedPair(std::vector<NakedPair*>& deadNakedPairs) -> NakedPair*
    {
        for (const std::string& type : {"Row", "Col", "Block"})
        {
            for (uint8_t unitID = 1; unitID <= global::order; unitID++) // go over all units
            {
                auto unit = this->getUnit(type, unitID);
                uint8_t numFreeFieldsInUnit = 0; // In der jeweiligen Unit muessen > 2 freie Felder sein
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
                                    auto* firstNakedPair = new NakedPair(field1, field2, i_1, i_2, type);
                                    bool isDead = false;
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
        this->_logTextArea->append(QStringLiteral("\nSearch for NakedPairs"));
        std::vector<NakedPair*> deadNakedPairs;
        while (true)
        {
            NakedPair* firstNakedPair = this->firstNakedPair(deadNakedPairs);
            if (firstNakedPair == nullptr)
            {
                break;
            }
            const uint8_t numCandsBeforeEliminatingFirstNakedPair = this->countCandidates();
            this->eliminateNakedPair(firstNakedPair);
            if (this->countCandidates() < numCandsBeforeEliminatingFirstNakedPair)
            {
                const QString msg =
                    "NakedPair {" +
                    QString::number(firstNakedPair->getCandidate1()) + "," +
                    QString::number(firstNakedPair->getCandidate2()) + "} in " +
                    QString::fromStdString(firstNakedPair->getType()) + " " +
                    QString::number(firstNakedPair->getUnitNumber()) + ": Fields (" +
                    QString::number(*firstNakedPair->getField1()->getRID()) + "," +
                    QString::number(*firstNakedPair->getField1()->getCID()) + ");(" +
                    QString::number(*firstNakedPair->getField2()->getRID()) + "," +
                    QString::number(*firstNakedPair->getField2()->getCID()) + ")";
                this->addStepToList(run, msg);
                this->_logTextArea->append(msg);
            }
        }
        // this->print();
        // this->printFields();
    }

    // Hidden Pair methods
    auto Sudoku::firstHiddenPair() -> HiddenSubset*
    {
        for (const std::string& type : {"Row", "Col", "Block"})
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
                                const std::vector<uint8_t> cands{i_1, i_2};
                                auto* firstHiddenPair = new HiddenSubset(hiddenPairFields, cands, type);
                                return firstHiddenPair;
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
        this->_logTextArea->append(QStringLiteral("\nSearch for HiddenPairs"));
        while (true)
        {
            HiddenSubset* firstHiddenPair = this->firstHiddenPair();
            if (firstHiddenPair == nullptr)
            {
                break;
            }
            const uint8_t numCandsBeforeEliminatingHiddenPair = this->countCandidates();
            const std::vector<uint8_t> hiddenPairCandidates = firstHiddenPair->getCandidates();
            retainAll(*firstHiddenPair->getFields().at(0)->getCandidates(), hiddenPairCandidates);
            retainAll(*firstHiddenPair->getFields().at(1)->getCandidates(), hiddenPairCandidates);

            if (this->countCandidates() < numCandsBeforeEliminatingHiddenPair)
            {
                const QString msg = "HiddenPair {" +
                                    QString::number(firstHiddenPair->getCandidates().at(0)) + "," +
                                    QString::number(firstHiddenPair->getCandidates().at(1)) + "} in " +
                                    QString::fromStdString(firstHiddenPair->getType()) + " " +
                                    QString::number(firstHiddenPair->getUnitNumber()) + ": Fields (" +
                                    QString::number(*firstHiddenPair->getFields().at(0)->getRID()) + "," +
                                    QString::number(*firstHiddenPair->getFields().at(0)->getCID()) + ");(" +
                                    QString::number(*firstHiddenPair->getFields().at(1)->getRID()) + "," +
                                    QString::number(*firstHiddenPair->getFields().at(1)->getCID()) + ")";
                this->addStepToList(run, msg);
                this->_logTextArea->append(msg);
            }
            // this->print();
            // this->printFields();
        }
    }

    // Naked Triple methods
    auto Sudoku::firstNakedTriple(std::vector<NakedTriple*>& deadNakedTriples) -> NakedTriple*
    {
        for (const std::string& type : {"Row", "Col", "Block"})
        {
            for (uint8_t unitID = 1; unitID <= global::order; unitID++) // go over all units
            {
                auto unit = this->getUnit(type, unitID);
                uint8_t numFreeFieldsInUnit = 0; // In der jeweiligen Unit muessen > 3 freie Felder sein
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
                                            for (const uint8_t cand : nonMatchCands)
                                            {
                                                if (std::find(field1->getCandidates()->begin(), field1->getCandidates()->end(), cand) != field1->getCandidates()->end())
                                                {
                                                    flIsNakedTriple = false;
                                                }
                                            }
                                            bool fmIsNakedTriple = true;
                                            for (const uint8_t cand : nonMatchCands)
                                            {
                                                if (std::find(field2->getCandidates()->begin(), field2->getCandidates()->end(), cand) != field2->getCandidates()->end())
                                                {
                                                    fmIsNakedTriple = false;
                                                }
                                            }
                                            bool fnIsNakedTriple = true;
                                            for (const uint8_t cand : nonMatchCands)
                                            {
                                                if (std::find(field3->getCandidates()->begin(), field3->getCandidates()->end(), cand) != field3->getCandidates()->end())
                                                {
                                                    fnIsNakedTriple = false;
                                                }
                                            }
                                            if (flIsNakedTriple && fmIsNakedTriple && fnIsNakedTriple && numFreeFieldsInUnit > 3)
                                            {
                                                auto* firstNakedTriple = new NakedTriple(field1, field2, field3, i_1, i_2, i_3, type);
                                                bool isDead = false;
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
            std::vector<uint8_t>* candidatesToRemove = nakedTriple->getField1()->getCandidates();

            for (const uint8_t candidate : *candidatesToRemove)
            {
                candidates->erase(std::remove(candidates->begin(), candidates->end(), candidate), candidates->end());
            }
        }
    }

    void Sudoku::eliminateNakedTriple(NakedTriple* nakedTriple)
    {
        if (*nakedTriple->getField1()->getRID() == *nakedTriple->getField2()->getRID() && *nakedTriple->getField1()->getRID() == *nakedTriple->getField3()->getRID() && *nakedTriple->getField2()->getRID() == *nakedTriple->getField3()->getRID())
        {
            this->eliminateCandidatesOfNakedTripleInUnit(nakedTriple, this->getRowByFieldID(*nakedTriple->getField1()->getFID()));
        }
        if (*nakedTriple->getField1()->getCID() == *nakedTriple->getField2()->getCID() && *nakedTriple->getField1()->getCID() == *nakedTriple->getField3()->getCID() && *nakedTriple->getField2()->getCID() == *nakedTriple->getField3()->getCID())
        {
            this->eliminateCandidatesOfNakedTripleInUnit(nakedTriple, this->getColByFieldID(*nakedTriple->getField1()->getFID()));
        }
        if (*nakedTriple->getField1()->getBID() == *nakedTriple->getField2()->getBID() && *nakedTriple->getField1()->getBID() == *nakedTriple->getField3()->getBID() && *nakedTriple->getField2()->getBID() == *nakedTriple->getField3()->getBID())
        {
            this->eliminateCandidatesOfNakedTripleInUnit(nakedTriple, this->getBlockByFieldID(*nakedTriple->getField1()->getFID()));
        }
    }

    void Sudoku::processNakedTriples(const uint8_t run)
    {
        this->_logTextArea->append(QStringLiteral("\nSearch for NakedTriples"));
        std::vector<NakedTriple*> deadNakedTriples;
        while (true)
        {
            NakedTriple* firstNakedTriple= this->firstNakedTriple(deadNakedTriples);
            if (firstNakedTriple == nullptr)
            {
                break;
            }

            const uint8_t numCandsBeforeEliminatingFirstNakedTriple = this->countCandidates();
            this->eliminateNakedTriple(firstNakedTriple);

            if (this->countCandidates() < numCandsBeforeEliminatingFirstNakedTriple)
            {
                const QString msg =
                    "NakedTriple {" +
                    QString::number(firstNakedTriple->getCandidate1()) + "," +
                    QString::number(firstNakedTriple->getCandidate2()) + "," +
                    QString::number(firstNakedTriple->getCandidate3()) + "} in " +
                    QString::fromStdString(firstNakedTriple->getType()) + " " +
                    QString::number(firstNakedTriple->getUnitNumber()) + ": Fields (" +
                    QString::number(*firstNakedTriple->getField1()->getRID()) + "," +
                    QString::number(*firstNakedTriple->getField1()->getCID()) + ");(" +
                    QString::number(*firstNakedTriple->getField2()->getRID()) + "," +
                    QString::number(*firstNakedTriple->getField2()->getCID()) + ");(" +
                    QString::number(*firstNakedTriple->getField3()->getRID()) + "," +
                    QString::number(*firstNakedTriple->getField3()->getCID()) + ")";
                this->addStepToList(run, msg);
                this->_logTextArea->append(msg);
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
        for (const std::string& type : {"Row", "Col", "Block"})
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
                                        //                                    loop:
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
                                                return new HiddenSubset(std::vector<Field*>({field1, field2, field3}), std::vector<uint8_t>({i_1, i_2, i_3}), type);
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
        this->_logTextArea->append(QStringLiteral("\nSearch for HiddenTriples"));
        while (true)
        {
            HiddenSubset* firstHiddenTriple = this->firstHiddenTriple();
            if (firstHiddenTriple == nullptr)
            {
                break;
            }
            const int numCandsBeforeEliminatingHiddenTriple = this->countCandidates();
            const std::vector<uint8_t> hiddenTripleCanidates = firstHiddenTriple->getCandidates();
            for (Field* field : firstHiddenTriple->getFields())
            {
                retainAll(*field->getCandidates(), hiddenTripleCanidates);
            }
            if (this->countCandidates() < numCandsBeforeEliminatingHiddenTriple)
            {
                const QString msg = "HiddenTriple {" +
                                    QString::number(firstHiddenTriple->getCandidates().at(0)) + "," +
                                    QString::number(firstHiddenTriple->getCandidates().at(1)) + "," +
                                    QString::number(firstHiddenTriple->getCandidates().at(2)) + "} in " +
                                    QString::fromStdString(firstHiddenTriple->getType()) + " " +
                                    QString::number(firstHiddenTriple->getUnitNumber()) + ": Fields (" +
                                    QString::number(*firstHiddenTriple->getFields().at(0)->getRID()) + "," +
                                    QString::number(*firstHiddenTriple->getFields().at(0)->getCID()) + ");(" +
                                    QString::number(*firstHiddenTriple->getFields().at(1)->getRID()) + "," +
                                    QString::number(*firstHiddenTriple->getFields().at(1)->getCID()) + ");(" +
                                    QString::number(*firstHiddenTriple->getFields().at(2)->getRID()) + "," +
                                    QString::number(*firstHiddenTriple->getFields().at(2)->getCID()) + ")";
                this->addStepToList(run, msg);
                this->_logTextArea->append(msg);
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
            const uint8_t numCandsBeforeBRC = this->countCandidates();
            this->removeCandidateIFromUnit(cand, containingCandidateI, lineIdOfFirst, type);
            if (this->countCandidates() < numCandsBeforeBRC)
            {
                const QString msg = "Block-in-" + QString::fromStdString(type) + " with {" + QString::number(cand) + "} in Block " + QString::number(blockID) + ": Only in " + QString::fromStdString(type) + " " + QString::number(lineIdOfFirst);
                this->_logTextArea->append(msg);
                this->addStepToList(run, msg);
            }
        }
    }

    // Block-Row-Checks (Pointing)
    void Sudoku::performBlockRowChecks(const uint8_t run)
    {
        this->_logTextArea->append(QStringLiteral("\nPerform Block-Line-Checks"));
        for (uint8_t blockID = 1; blockID <= global::order; blockID++)
        {
            const std::array<Field*, global::order> block = this->getBlockByBlockID(blockID);
            for (uint8_t cand = 1; cand <= global::order; cand++)
            {
                std::vector<Field*> containingCandidateI = this->findFieldsInUnitContainingCandidateI(block, cand);
                if (!containingCandidateI.empty())
                {
                    for (const std::string& type : {"Row", "Col"})
                    {
                        if (type == "Row")
                        {
                            const uint8_t rowIdOfFirst = *containingCandidateI[0]->getRID();
                            bool allCandidatesInSameRow = false;
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
                            const uint8_t colIdOfFirst = *containingCandidateI[0]->getCID();
                            bool allCandidatesInSameCol = false;
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
        this->_logTextArea->append(QStringLiteral("\nPerform Line-Block-Checks"));
        for (const std::string& type : {"Row", "Col"})
        {
            for (uint8_t lineID = 1; lineID <= global::order; lineID++) // Iterate over each row/col
            {
                auto unit = this->getUnit(type, lineID);
                for (uint8_t i = 1; i <= global::order; i++)
                {
                    std::vector<Field*> containingCandidateI = this->findFieldsInUnitContainingCandidateI(unit, i);
                    if (!containingCandidateI.empty()) // proceed only if there exists at least one field with candidate i
                    {
                        const uint8_t blockIdOfFirst = *containingCandidateI[0]->getBID();
                        bool allCandidatesInSameBlock = false;
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
                            const uint8_t numCandsBeforeRBC = this->countCandidates();
                            this->removeCandidateIFromUnit(i, containingCandidateI, blockIdOfFirst, "Block"); // remove candidate i from block
                            if (this->countCandidates() < numCandsBeforeRBC)
                            {
                                const QString msg = QString::fromStdString(type) + "-in-Block with {" + QString::number(i) + "} in " +
                                                    QString::fromStdString(type) + " " + QString::number(lineID) + ": " +
                                                    "Only in Block " + QString::number(blockIdOfFirst);
                                this->_logTextArea->append(msg);
                                this->addStepToList(run, msg);
                            }
                        }
                    }
                }
            }
        }
        // this->print();
        // this->printFields();
    }

    // Main Solving routine
    void Sudoku::solve(const std::string& name)
    {
        this->_logTextArea->clear();
        this->_logTextArea->append("START SOLVING SUDOKU '" + QString::fromStdString(name) + "'");
        this->_logTextArea->append("Initial free fields: " + QString::number(this->getFreeFields().size()));
        this->_logTextArea->append("Initial candidates: " + QString::number(this->countCandidates()) + "\n");

        uint8_t run = 0;

        addStepToList(run, QStringLiteral("Initial status"));

        std::chrono::high_resolution_clock::time_point t_1;
        const std::chrono::high_resolution_clock::time_point t_0 = std::chrono::high_resolution_clock::now();

        // Main solving loop
        while (true)
        {
            run++;

            const auto freeFieldsBeforeRun = this->getFreeFields().size();
            const auto numCandsBeforeRun = this->countCandidates();

            this->_logTextArea->append("BEGIN Run No. " + QString::number(run) + ":\n");

            // Process Singles
            this->processNakedSingles(run);  // Check Naked Singles
            this->processHiddenSingles(run); // Check Hidden Singles
            const auto freeFieldsAfterSingles = this->getFreeFields().size();

            // If no (more) Singles exist -> Search for Naked and Hidden Pairs
            if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && !this->getFreeFields().empty())
            {
                if (freeFieldsAfterSingles < freeFieldsBeforeRun)
                {
                    this->_logTextArea->append(QStringLiteral("\nNo further Singles found.\nProceeding with Pairs..."));
                }
                else
                {
                    this->_logTextArea->append(QStringLiteral("\nNo Singles found.\nProceeding with pairs..."));
                }

                const uint8_t numCandsBeforePairs = this->countCandidates();
                this->processNakedPairs(run);  // Check Naked Pairs
                this->processHiddenPairs(run); // Check Hidden Pairs
                const uint8_t numCandsAfterPairs = this->countCandidates();

                // If Pair techniques didn't produce Singles -> search for Naked and Hidden Triples
                if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && !this->getFreeFields().empty())
                {
                    if (numCandsAfterPairs < numCandsBeforePairs)
                    {
                        this->_logTextArea->append(QStringLiteral("\nPair techniques didn't produce Singles.\nProceeding with Triples..."));
                    }
                    else
                    {
                        this->_logTextArea->append(QStringLiteral("\nNo Pairs found.\nProceeding with Triples..."));
                    }

                    const uint8_t numCandsBeforeTriples = this->countCandidates();
                    this->processNakedTriples(run);  // Check Naked Triples
                    this->processHiddenTriples(run); // Check Hidden Triples
                    const uint8_t numCandsAfterTriples = this->countCandidates();

                    // If pair/triple techniques didn't produce Singles -> go for Row-Block-Checks and Block-Row-Checks
                    if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && !this->getFreeFields().empty())
                    {
                        if (numCandsAfterTriples < numCandsBeforeTriples)
                        {
                            this->_logTextArea->append(QStringLiteral("\nTriple techniques didn't produce Singles.\nProceeding with LockedCandidates..."));
                        }
                        else
                        {
                            this->_logTextArea->append(QStringLiteral("\nNo Triples found.\nProceeding with LockedCandidates..."));
                        }

                        const uint8_t numCandsBeforeLockedCands = this->countCandidates();
                        this->performBlockRowChecks(run); // Perfom Block-Row-Checks (Pointing)
                        this->performRowBlockChecks(run); // Peform Row-Block-Checks (Claiming)
                        const uint8_t numCandsAfterLockedCands = this->countCandidates();

                        // If pair/triple/LockedCands didn't produce Singles -> go for further technique
                        if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && !this->getFreeFields().empty())
                        {
                            if (numCandsAfterLockedCands < numCandsBeforeLockedCands)
                            {
                                this->_logTextArea->append(QStringLiteral("\nLockedCandidates didn't produce Singles.\nNeed further techniques..."));
                            }
                            else
                            {
                                this->_logTextArea->append(QStringLiteral("\nNo LockedCandidates found.\nNeed further techniques..."));
                            }

                            const uint8_t numCandsBeforeFurtherTechnique = this->countCandidates();
                            // to do implement further techniques
                            const uint8_t numCandsAfterFurtherTechnique = this->countCandidates();

                            // If further technique didn't produce Singles -> go for another further technique
                            if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && !this->getFreeFields().empty())
                            {
                                if (numCandsAfterFurtherTechnique < numCandsBeforeFurtherTechnique)
                                {
                                    this->_logTextArea->append(QStringLiteral("\nFurther technique didn't produce Singles.\nNo further techniques implemented..."));
                                }
                                else
                                {
                                    this->_logTextArea->append(QStringLiteral("\nNo more further technique.\nNo further techniques implemented..."));
                                }
                            }
                            else
                            {
                                this->_logTextArea->append(QStringLiteral("\nFurther technique produced Singles.\nProceeding with next run..."));
                            }
                        }
                        else
                        {
                            this->_logTextArea->append(QStringLiteral("\nLockedCandidates produced Singles.\nProceeding with next run..."));
                        }
                    }
                    else
                    {
                        this->_logTextArea->append(QStringLiteral("\nTriple techniques produced Singles.\nProceeding with next run..."));
                    }
                }
                else
                {
                    this->_logTextArea->append(QStringLiteral("\nPair techniques produced Singles.\nProceeding with next run..."));
                }
            }
            else
            {
                if (!this->getFreeFields().empty())
                {
                    this->_logTextArea->append(QStringLiteral("\nNo more HiddenSingles, but new NakedSingles.\nProceeding with next run..."));
                }
            }

            const auto freeFieldsAfterRun = this->getFreeFields().size();
            const auto numCandsAfterRun = this->countCandidates();

            this->_logTextArea->append("\nEND Run No. " + QString::number(run) + ":");
            this->_logTextArea->append(QString::number(freeFieldsAfterRun) + " free fields, " + QString::number(numCandsAfterRun) + " candidates\n‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒\n");

            if (numCandsAfterRun == 0)
            {
                t_1 = std::chrono::high_resolution_clock::now();
                this->_logTextArea->append(QStringLiteral("Finished: No free fields remaining\n"));
                break;
            }

            if (numCandsBeforeRun == numCandsAfterRun)
            {
                t_1 = std::chrono::high_resolution_clock::now();
                this->_logTextArea->append(QStringLiteral("Abort: No more solving techniques implemented\n"));
                break;
            }
        }

        if (this->getFreeFields().empty())
        {
            this->_logTextArea->append("SUDOKU '" + QString::fromStdString(name) + "' SUCESSFULLY SOLVED!\n");
        }
        else
        {
            this->_logTextArea->append("SOLVING SUDOKU '" + QString::fromStdString(name) + "' FAILED!\n");
            this->_logTextArea->append(QStringLiteral("\nAbort status:"));
            this->print();
            this->printFields();
            this->_logTextArea->append("\nAbort with " + QString::number(this->getFreeFields().size()) + " free fields and " + QString::number(this->countCandidates()) + " candidates\n");
        }

        this->_logTextArea->append("Elapsed time: " + QString::number(std::chrono::duration<double, std::milli>(t_1 - t_0).count(), 'f', 1) + " ms\n");
    }

    void Sudoku::addStepToList(const uint8_t run, const QString& type)
    {
        _steps.push_back(_grid);
        _foundInRunNo.push_back(run);
        _foundByType.push_back(type);
    }

    void Sudoku::print() const
    {
        QString printString;
        QTextStream printStream(&printString);

        printStream << "col  1  2  3   4  5  6   7  8  9  " << Qt::endl;
        printStream << "row ----------------------------- " << Qt::endl;
        uint8_t rowID = 1;
        for (uint8_t rID = 1; rID <= global::order; ++rID)
        {
            QString str = " " + QString::number(rowID) + " |";
            uint8_t colID = 1;
            for (uint8_t cID = 1; cID <= global::order; ++cID)
            {
                const uint8_t val = *_grid.at(rID - 1).at(cID - 1).getVal();
                if (val != 0)
                {
                    str += " " + QString::number(val) + " ";
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
        QTextStream printStream(&printString);
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
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
