//#include <algorithm>
//#include <iterator>
// #include <vector>
#include <QtCore/QString>
#include <QtCore/QTextStream>
//#include <iomanip>
//#include <iostream>

//#include "HiddenSingle.hpp"
//#include "Main.hpp"
//#include "gui/QLogTextBrowser.h"
//#include "sudoku/NakedPair.h"
//#include "sudoku/NakedTriple.h"
#include "sudoku/Sudoku.h"

//using std::cout;
//using std::endl;
// using std::vector;

namespace sudoku
{
    Sudoku::Sudoku(const uint8_t* vals, QLogTextBrowser& logTextArea)
        : _logTextArea(&logTextArea)
    {
        // _logTextArea->append("Constructor!");
        // cout << "   field @        |fID @                |rID@                |cID@                |bID@                |cands@               |val@                |" << endl;
        // Fill values
        uint8_t fID = 1;
        for (uint8_t rID = 1; rID <= order; rID++)
        {
            for (uint8_t cID = 1; cID <= order; cID++)
            {
                // cout << "Constructor: val=" << vals[id-1] << endl;
                _grid[rID - 1][cID - 1] = Field(fID, vals[fID - 1]);
                // cout << "S: " << &_grid[r - 1][c - 1] << " | ";
                // cout << "" << std::fixed << std::setw(2) << *_grid[r - 1][c - 1].getFID() << " @ " << _grid[r - 1][c - 1].getFID() << " | ";
                // cout << "" << *_grid[r - 1][c - 1].getRID() << " @ " << _grid[r - 1][c - 1].getRID() << " | ";
                // cout << "" << *_grid[r - 1][c - 1].getCID() << " @ " << _grid[r - 1][c - 1].getCID() << " | ";
                // cout << "" << *_grid[r - 1][c - 1].getBID() << " @ " << _grid[r - 1][c - 1].getBID() << " | ";
                // cout << "" << _grid[r - 1][c - 1].candidates2QString().toStdString() << " @ " << _grid[r - 1][c - 1].getCandidates() << " | ";
                // cout << "" << *_grid[r - 1][c - 1].getVal() << " @ " << _grid[r - 1][c - 1].getVal() << " | ";
                // cout << endl;
                fID++;
            }
        }

        // Fill candidates
        // Initially: 1-9 are possible candidates.
        for (uint8_t rID = 1; rID <= order; rID++)
        {
            for (uint8_t cID = 1; cID <= order; cID++)
            {
                Field* f = &_grid[rID - 1][cID - 1];
                if (*f->getVal() == 0)
                {
                    std::vector<uint8_t> candidates;
                    for (uint8_t cand = 1; cand <= order; cand++)
                    {
                        candidates.push_back(cand);
                    }
                    f->setCandidates(&candidates);
                }
            }
        }

        // Then: For each field.candidates(): go through rows, cols, blocks and
        // remove existing vals from cands

        // rows
        for (uint8_t rID = 1; rID <= order; rID++)
        {
            for (uint8_t cID = 1; cID <= order; cID++)
            {
                Field* field = &_grid[rID - 1][cID - 1];
                vector<uint8_t>* candidates = field->getCandidates();

                array<Field*, order> row = getRowByFieldID(*field->getFID());
                for (uint8_t rID = 1; rID <= order; rID++)
                {
                    const uint8_t* val = row[rID - 1]->getVal();
                    if (*val != 0)
                    {
                        // cout << "FOUND " << *val << endl;
                        candidates->erase(std::remove(candidates->begin(), candidates->end(), *val), candidates->end());
                    }
                }
                // cout << "(" << f->getRID() << "," << f->getCID() << "): " << f->candidates2QString().toStdString() << endl;
            }
        }

        // cols
        for (uint8_t cID = 1; cID <= order; cID++)
        {
            for (uint8_t rID = 1; rID <= order; rID++)
            {
                Field* field = &_grid[rID - 1][cID - 1];
                vector<uint8_t>* candidates = field->getCandidates();

                array<Field*, order> col = getColByFieldID(*field->getFID());
                for (uint8_t cID = 1; cID <= order; cID++)
                {
                    const uint8_t* val = col[cID - 1]->getVal();
                    if (*val != 0)
                    {
                        // cout << "FOUND " << *val << endl;
                        candidates->erase(std::remove(candidates->begin(), candidates->end(), *val), candidates->end());
                    }
                }
                // cout << "(" << f->getRID() << "," << f->getCID() << "): " << f->candidates2QString().toStdString() << endl;
            }
        }

        // blocks
        for (uint8_t rID = 1; rID <= order; rID++)
        {
            for (uint8_t cID = 1; cID <= order; cID++)
            {
                Field* field = &_grid[rID - 1][cID - 1];
                vector<uint8_t>* candidates = field->getCandidates();

                array<Field*, order> block = getBlockByFieldID(*field->getFID());
                for (uint8_t bID = 1; bID <= order; bID++)
                {
                    const uint8_t* val = block[bID - 1]->getVal();
                    if (*val != 0)
                    {
                        // cout << "FOUND " << *val << endl;
                        candidates->erase(std::remove(candidates->begin(), candidates->end(), *val), candidates->end());
                    }
                }
                // cout << "(" << f->getRID() << "," << f->getCID() << "): " << f->candidates2QString().toStdString() << endl;
            }
        }
    }

    Sudoku::~Sudoku()
    {
    }

    vector<array<array<Field, order>, order>>* Sudoku::getSteps()
    {
        return &_steps;
    }

    QLogTextBrowser* Sudoku::getLogTextArea()
    {
        return _logTextArea;
    }

    vector<uint8_t>* Sudoku::getFoundInRunNo()
    {
        return &_foundInRunNo;
    }

    vector<QString>* Sudoku::getFoundByType()
    {
        return &_foundByType;
    }

    //Field* Sudoku::createEmptyGrid()
    //{
    //    Field* grid[9][9];
    //    uint8_t id = 1;
    //    for (uint8_t r = 1; r <= 9; r++)
    //    {
    //        for (uint8_t c = 1; c <= 9; c++)
    //        {
    //            grid[r - 1][c - 1] = Field(id);
    //            id++;
    //        }
    //    }
    //    _foundInRunNo.clear();
    //    _foundByType.clear();
    //    return grid;
    //}

    Field* Sudoku::getFieldByCoord(const uint8_t& rID, const uint8_t& cID)
    {
        // cout << "Sudoku: val = " << _grid[rID - 1][cID - 1].getVal() << endl;
        return &_grid[rID - 1][cID - 1];
    }

    Field* Sudoku::getFieldByFieldID(const uint8_t& fID)
    {
        const uint8_t rID = (fID - 1) / order + 1;
        const uint8_t cID = (fID - 1) % order + 1;
        return getFieldByCoord(rID, cID);
    }

    array<Field*, order> Sudoku::getRowByRowID(const uint8_t& rID)
    {
        array<Field*, order> row;
        for (uint8_t cID = 1; cID <= order; cID++)
        {
            row[cID - 1] = &_grid[rID - 1][cID - 1];
        }
        return row;
    }

    array<Field*, order> Sudoku::getRowByFieldID(const uint8_t& fID)
    {
        const uint8_t rID = (fID - 1) / order + 1;
        return getRowByRowID(rID);
    }

    array<Field*, order> Sudoku::getColByColID(const uint8_t& cID)
    {

        array<Field*, order> col;
        for (uint8_t rID = 1; rID <= order; rID++)
        {
            col[rID - 1] = &_grid[rID - 1][cID - 1];
        }
        return col;
    }

    array<Field*, order> Sudoku::getColByFieldID(const uint8_t& fID)
    {
        const uint8_t cID = (fID - 1) % order + 1;
        return getColByColID(cID);
    }

    // Field** Sudoku::getBlockByUpperLeftField(uint8_t r0, uint8_t c0)
    // {
    //    Field** block = new Field*[3];
    //    for (uint8_t i = 0; i < 3; i++)
    //    {
    //        block[i] = new Field[3];
    //        for (uint8_t j = 0; j < 3; j++)
    //        {
    //            block[i][j] = grid[r0 + i][c0 + j];
    //        }
    //    }
    //    return block;
    // }

    array<Field*, order> Sudoku::getBlockByBlockID(const uint8_t& bID)
    {
        array<Field*, order> block;

        uint8_t const r0 = (bID - 1) / 3 * 3;
        uint8_t const c0 = (bID - 1) % 3 * 3;

        uint8_t i = 0;
        for (uint8_t r = r0; r < r0 + 3; r++)
        {
            for (uint8_t c = c0; c < c0 + 3; c++)
            {
                block[i] = &_grid[r][c];
                i++;
            }
        }
        return block;
    }

    array<Field*, order> Sudoku::getBlockByFieldID(const uint8_t& fID)
    {
        // clang-format off
        if      (fID ==  1 || fID ==  2 || fID ==  3 || fID == 10 || fID == 11 || fID == 12 || fID == 19 || fID == 20 || fID == 21) return getBlockByBlockID(1);
        else if (fID ==  4 || fID ==  5 || fID ==  6 || fID == 13 || fID == 14 || fID == 15 || fID == 22 || fID == 23 || fID == 24) return getBlockByBlockID(2);
        else if (fID ==  7 || fID ==  8 || fID ==  9 || fID == 16 || fID == 17 || fID == 18 || fID == 25 || fID == 26 || fID == 27) return getBlockByBlockID(3);
        else if (fID == 28 || fID == 29 || fID == 30 || fID == 37 || fID == 38 || fID == 39 || fID == 46 || fID == 47 || fID == 48) return getBlockByBlockID(4);
        else if (fID == 31 || fID == 32 || fID == 33 || fID == 40 || fID == 41 || fID == 42 || fID == 49 || fID == 50 || fID == 51) return getBlockByBlockID(5);
        else if (fID == 34 || fID == 35 || fID == 36 || fID == 43 || fID == 44 || fID == 45 || fID == 52 || fID == 53 || fID == 54) return getBlockByBlockID(6);
        else if (fID == 55 || fID == 56 || fID == 57 || fID == 64 || fID == 65 || fID == 66 || fID == 73 || fID == 74 || fID == 75) return getBlockByBlockID(7);
        else if (fID == 58 || fID == 59 || fID == 60 || fID == 67 || fID == 68 || fID == 69 || fID == 76 || fID == 77 || fID == 78) return getBlockByBlockID(8);
        else                                                                                                                        return getBlockByBlockID(9);
        // clang-format on
    }

    vector<Field*> Sudoku::getFreeFields()
    {
        vector<Field*> freeFields;
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

    uint8_t Sudoku::countCandidates()
    {
        uint8_t numCands = 0;
        for (uint8_t rID = 1; rID <= order; rID++)
        {
            for (uint8_t cID = 1; cID <= order; cID++)
            {
                numCands += _grid[rID - 1][cID - 1].getCandidates()->size();
            }
        }
        return numCands;
    }

    void Sudoku::filldAndEliminate(Field* field)
    {
        // set the value of f to val
        if (field->getCandidates()->size() > 1)
        {
            QString const errorMessage = "Error filling Field (" + QString::number(*field->getRID()) + "," + QString::number(*field->getCID()) + "): Field has more than one candidate!";
            this->_logTextArea->append(errorMessage);
            // Display an error message
            // QMessageBox::critical(logTextArea, "Error filling Field", errorMessage);
            return;
        }
        uint8_t const val = (*field->getCandidates())[0];
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
    Field* Sudoku::firstNakedSingle()
    {
        for (uint8_t rID = 1; rID <= order; rID++)
        {
            for (uint8_t cID = 1; cID <= order; cID++)
            {
                Field* field = &_grid[rID - 1][cID - 1];
                if (field->getCandidates()->size() == 1)
                {
                    return field;
                }
            }
        }
        return nullptr;
    }

    void Sudoku::processNakedSingles(uint8_t run)
    {
        this->_logTextArea->append("Search for NakedSingles");
        while (true)
        {
            Field* firstNakedSingle = this->firstNakedSingle();
            if (firstNakedSingle == nullptr)
                break;
            this->filldAndEliminate(firstNakedSingle);
            QString const msg =
                "NakedSingle {" + QString::number(*firstNakedSingle->getVal()) +
                "} in Field (" + QString::number(*firstNakedSingle->getRID()) + "," +
                QString::number(*firstNakedSingle->getCID()) + ")";
            this->_logTextArea->append(msg);
            this->addStepToList(run, msg);
        }
        // this->print();
        // this->printFields();
    }

    bool Sudoku::fieldContainsCandidate(std::vector<uint8_t>* candidates, uint8_t cand)
    {
        return std::find(candidates->begin(), candidates->end(), cand) != candidates->end();
    }

    uint8_t* Sudoku::candidateOccurrencesInUnit(array<Field*, order> unit)
    {
        uint8_t* candidateOccurrencesInUnit = new uint8_t[order](); // Initialize array elements to 0
        for (uint8_t cand = 1; cand <= order; cand++)
        {
            uint8_t count = 0;
            for (uint8_t unitID = 1; unitID <= order; unitID++)
            {
                if (fieldContainsCandidate(unit[unitID - 1]->getCandidates(), cand))
                {
                    count++;
                }
            }
            candidateOccurrencesInUnit[cand - 1] = count;
        }
        return candidateOccurrencesInUnit;
    }

    HiddenSingle* Sudoku::firstHiddenSingle()
    {
        // Reihenfolge: Erst Reihen, dann Spalten, dann Bloecke
        // nach HiddenSingle durchsuchen
        std::string const types[] = {"Row", "Col", "Block"};
        for (const std::string& type : types)
        {
            for (uint8_t unitID = 1; unitID <= order; unitID++)
            {
                array<Field*, order> unit;
                if (type == "Row")
                    unit = getRowByRowID(unitID);
                else if (type == "Col")
                    unit = getColByColID(unitID);
                else if (type == "Block")
                    unit = getBlockByBlockID(unitID);
                uint8_t* candsInUnit = candidateOccurrencesInUnit(unit);
                // Finde Zahlen "i", die nur 1x vorkommen
                for (uint8_t i = 1; i <= order; i++)
                {
                    if (candsInUnit[i - 1] == 1)
                    {
                        // Pruefe Felder in Reihe i: Welches Feld enthaelt Zahl "num"?
                        for (uint8_t fID = 1; fID <= order; fID++)
                        {
                            Field* field = unit[fID - 1];
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

    void Sudoku::processHiddenSingles(uint8_t run)
    {
        this->_logTextArea->append("\nSearch for HiddenSingles");
        while (true)
        {
            HiddenSingle* firstHiddenSingle = this->firstHiddenSingle();
            if (firstHiddenSingle == nullptr)
                break;

            std::vector<uint8_t> const cand(1, *firstHiddenSingle->getCandidate());
            firstHiddenSingle->getField()->setCandidates(&cand);
            this->filldAndEliminate(firstHiddenSingle->getField());

            QString const msg =
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
    NakedPair* Sudoku::firstNakedPair(std::vector<NakedPair*>& deadNakedPairs)
    {
        for (const std::string& type : {"Row", "Col", "Block"})
        {
            // this->_logTextArea->append("firstNakedPair()::type = " + QString::fromStdString(type));
            for (uint8_t unitID = 1; unitID <= order; unitID++) // go over all units
            {
                array<Field*, order> unit;
                if (type == "Row")
                    unit = getRowByRowID(unitID);
                else if (type == "Col")
                    unit = getColByColID(unitID);
                else if (type == "Block")
                    unit = getBlockByBlockID(unitID);

                uint8_t numFreeFieldsInUnit = 0; // In der jeweiligen Unit muessen > 2 freie Felder sein
                for (uint8_t fID = 1; fID <= order; fID++)
                {
                    if (*unit[fID - 1]->getVal() == 0)
                        numFreeFieldsInUnit++;
                }

                for (uint8_t i1 = 1; i1 <= order-1; i1++) // Alle Zahlenpaare {1,2} bis {8,9}
                {
                    for (uint8_t i2 = i1 + 1; i2 <= order; i2++)
                    {
                        for (uint8_t fID1 = 1; fID1 <= order-1; fID1++) // Alle Feld-Kombinationen in unit
                        {
                            for (uint8_t fID2 = fID1 + 1; fID2 <= order; fID2++)
                            {
                                //                                this->_logTextArea->append("type=" + QString::fromStdString(type) + " , {i,j}={" + QString::number(i) + "," + QString::number(j) + "} , Felder " + QString::number(k) + "," + QString::number(l));
                                Field* fk = unit[fID1 - 1];
                                Field* fl = unit[fID2 - 1];
                                //                                if (run == 1)
                                //                                {
                                //                                    this->_logTextArea->append(QString::fromStdString(type));
                                //                                    fk->printField(*_logTextArea);
                                //                                    fl->printField(*_logTextArea);
                                //                                    this->_logTextArea->append("numFreeFieldsInInut = " + QString::number(numFreeFieldsInUnit));
                                //                                }
                                // Kandidatenlisten der beiden Felder muessen Laenge 2 haben und
                                // die einzigen Kandidaten muessen die Zahlen i und j sein
                                uint8_t candsij[] = {i1, i2};
                                if (fk->getCandidates()->size() == 2 &&
                                    fl->getCandidates()->size() == 2 &&
                                    // containsAll(fk->getCandidates(), {i, j}) &&
                                    // containsAll(fl->getCandidates(), {i, j}) &&
                                    //                                    std::includes(fk->getCandidates()->begin(), fk->getCandidates()->end(), candsij.begin(), candsij.end()) &&
                                    //                                    std::includes(fl->getCandidates()->begin(), fl->getCandidates()->end(), candsij.begin(), candsij.end()) &&
                                    std::equal(fk->getCandidates()->begin(), fk->getCandidates()->end(), candsij) &&
                                    std::equal(fl->getCandidates()->begin(), fl->getCandidates()->end(), candsij) &&
                                    numFreeFieldsInUnit > 2)
                                {
                                    NakedPair* firstNakedPair = new NakedPair(fk, fl, i1, i2, type);
                                    bool isDead = false;
                                    for (NakedPair* dead : deadNakedPairs)
                                    {
                                        if (dead->getField1() == firstNakedPair->getField1() &&
                                            dead->getField2() == firstNakedPair->getField2() &&
                                            dead->getType() == firstNakedPair->getType())
                                        {
                                            isDead = true;
                                            // this->_logTextArea->append("IS DEAD!");
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
        //        this->_logTextArea->append("firstNakedPair() == nullptr");
        return nullptr;
    }

    void Sudoku::eliminateCandidatesOfNakedPairInUnit(NakedPair* nakedPair, std::array<Field*, order> unit)
    {
        for (Field* f : unit)
        {
            if (f->getFID() == nakedPair->getField1()->getFID() ||
                f->getFID() == nakedPair->getField2()->getFID() ||
                *f->getVal() != 0)
            {
                continue;
            }

            std::vector<uint8_t>* candidates = f->getCandidates();
            std::vector<uint8_t>* candidatesToRemove = nakedPair->getField1()->getCandidates();

            for (uint8_t const candidate : *candidatesToRemove)
            {
                candidates->erase(std::remove(candidates->begin(), candidates->end(), candidate), candidates->end());
            }
        }
    }

    void Sudoku::eliminateNakedPair(NakedPair* nakedPair)
    {
        if (*nakedPair->getField1()->getRID() == *nakedPair->getField2()->getRID())
            eliminateCandidatesOfNakedPairInUnit(nakedPair, getRowByFieldID(*nakedPair->getField1()->getFID()));
        if (*nakedPair->getField1()->getCID() == *nakedPair->getField2()->getCID())
            eliminateCandidatesOfNakedPairInUnit(nakedPair, getColByFieldID(*nakedPair->getField1()->getFID()));
        if (*nakedPair->getField1()->getBID() == *nakedPair->getField2()->getBID())
            eliminateCandidatesOfNakedPairInUnit(nakedPair, getBlockByFieldID(*nakedPair->getField1()->getFID()));
    }

    void Sudoku::processNakedPairs(uint8_t run)
    {
        this->_logTextArea->append("\nSearch for NakedPairs");
        std::vector<NakedPair*> deadNakedPairs;
        while (true)
        {
            NakedPair* firstNakedPair = this->firstNakedPair(deadNakedPairs);
            if (firstNakedPair == nullptr)
                break;

            uint8_t const numCandsBeforeEliminatingFirstNakedPair = this->countCandidates();
            //            this->_logTextArea->append("NOW ELIMINATING: NakedPair {" +
            //                                       QString::number(firstNakedPair->getCandidate1()) + "," +
            //                                       QString::number(firstNakedPair->getCandidate2()) + "} in " +
            //                                       QString::fromStdString(firstNakedPair->getType()) + " " +
            //                                       QString::number(firstNakedPair->getUnitNumber()) + ": Fields (" +
            //                                       QString::number(*firstNakedPair->getField1()->getRID()) + "," +
            //                                       QString::number(*firstNakedPair->getField1()->getCID()) + ");(" +
            //                                       QString::number(*firstNakedPair->getField2()->getRID()) + "," +
            //                                       QString::number(*firstNakedPair->getField2()->getCID()) + ")");
            this->eliminateNakedPair(firstNakedPair);
            if (this->countCandidates() < numCandsBeforeEliminatingFirstNakedPair)
            {
                QString const msg =
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
    HiddenSubset* Sudoku::firstHiddenPair()
    {
        for (const std::string& type : {"Row", "Col", "Block"})
        {
            for (uint8_t unitID = 1; unitID <= order; unitID++) // go over all units
            {
                array<Field*, order> unit;
                if (type == "Row")
                    unit = getRowByRowID(unitID);
                else if (type == "Col")
                    unit = getColByColID(unitID);
                else if (type == "Block")
                    unit = getBlockByBlockID(unitID);
                uint8_t* candidateOccurrences = candidateOccurrencesInUnit(unit);
                for (uint8_t i1 = 1; i1 <= order-1; i1++) // Alle Zahlenpaare {1,2} bis {8,9}
                {
                    for (uint8_t i2 = i1 + 1; i2 <= order; i2++)
                    {
                        // Wenn i und j genau 2 mal vorkommen
                        if (candidateOccurrences[i1 - 1] == 2 && candidateOccurrences[i2 - 1] == 2)
                        {
                            // Finde Felder, in denen i und j vorkommen
                            std::vector<Field*> hiddenPairFields;
                            for (Field* f : unit)
                            {
                                if (std::find(f->getCandidates()->begin(), f->getCandidates()->end(), i1) != f->getCandidates()->end() &&
                                    std::find(f->getCandidates()->begin(), f->getCandidates()->end(), i2) != f->getCandidates()->end())
                                {
                                    hiddenPairFields.push_back(f);
                                }
                            }
                            if (hiddenPairFields.size() == 2 && hiddenPairFields.at(0) != nullptr && hiddenPairFields.at(1) != nullptr &&
                                (hiddenPairFields.at(0)->getCandidates()->size() > 2 || hiddenPairFields.at(1)->getCandidates()->size() > 2))
                            {
                                std::vector<uint8_t> const cands({i1, i2});
                                HiddenSubset* firstHiddenPair = new HiddenSubset(hiddenPairFields, cands, type);
                                //                                this->_logTextArea->append("HIDDEN PAIR {" +
                                //                                                           QString::number(firstHiddenPair->getCandidates().at(0)) + "," +
                                //                                                           QString::number(firstHiddenPair->getCandidates().at(1)) + "} in " +
                                //                                                           QString::fromStdString(firstHiddenPair->getType()) + " " +
                                //                                                           QString::number(firstHiddenPair->getUnitNumber()) + ": Fields (" +
                                //                                                           QString::number(*firstHiddenPair->getFields().at(0)->getRID()) + "," +
                                //                                                           QString::number(*firstHiddenPair->getFields().at(0)->getCID()) + ");(" +
                                //                                                           QString::number(*firstHiddenPair->getFields().at(1)->getRID()) + "," +
                                //                                                           QString::number(*firstHiddenPair->getFields().at(1)->getCID()) + ")");
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

    void Sudoku::processHiddenPairs(uint8_t run)
    {
        this->_logTextArea->append("\nSearch for HiddenPairs");
        while (true)
        {
            HiddenSubset* firstHiddenPair = this->firstHiddenPair();
            if (firstHiddenPair == nullptr)
                break;

            uint8_t const numCandsBeforeEliminatingHiddenPair = this->countCandidates();

            //            this->_logTextArea->append("Before Eliminating:");
            //            firstHiddenPair->getFields().at(0)->printField(*_logTextArea);
            //            firstHiddenPair->getFields().at(1)->printField(*_logTextArea);

            std::vector<uint8_t> const hiddenPairCandidates = firstHiddenPair->getCandidates();
            retainAll(*firstHiddenPair->getFields().at(0)->getCandidates(), hiddenPairCandidates);
            retainAll(*firstHiddenPair->getFields().at(1)->getCandidates(), hiddenPairCandidates);

            //            this->_logTextArea->append("After Eliminating:");
            //            firstHiddenPair->getFields().at(0)->printField(*_logTextArea);
            //            firstHiddenPair->getFields().at(1)->printField(*_logTextArea);

            if (this->countCandidates() < numCandsBeforeEliminatingHiddenPair)
            {
                QString const msg = "HiddenPair {" +
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
            //            this->print();
            //            this->printFields();
        }
    }

    // Naked Triple methods
    NakedTriple* Sudoku::firstNakedTriple(std::vector<NakedTriple*>& deadNakedTriples)
    {
        for (const std::string& type : {"Row", "Col", "Block"})
        {
            for (uint8_t unitID = 1; unitID <= order; unitID++) // go over all units
            {
                array<Field*, order> unit;
                if (type == "Row")
                    unit = getRowByRowID(unitID);
                else if (type == "Col")
                    unit = getColByColID(unitID);
                else if (type == "Block")
                    unit = getBlockByBlockID(unitID);

                uint8_t numFreeFieldsInUnit = 0; // In der jeweiligen Unit muessen > 3 freie Felder sein
                for (uint8_t fID = 1; fID <= order; fID++)
                {
                    if (*unit[fID - 1]->getVal() == 0)
                        numFreeFieldsInUnit++;
                }

                for (uint8_t i=1; i<=order-2; i++) // Alle Zahlen-Tripel {1,2,3} bis {7,8,9}
                {
                    for (uint8_t j=i+1; j<=order-1; j++)
                    {
                        for (uint8_t k=j+1; k<=order; k++)
                        {
                            std::vector<uint8_t> nonMatchCands;
                            // Kandidatenlisten aller drei Felder duerfen ausschliesslich die Zahlen i,j,k enthalten
                            for (uint8_t c=1; c<=order; c++)
                            {
                                if (c!=i && c!=j && c!=k)
                                {
                                    nonMatchCands.push_back(c);
                                }
                            }
                            for (uint8_t l=1; l<=order-2; l++) // Alle Feld-Kombinationen in unit
                            {
                                for (uint8_t m=l+1; m<=order-1; m++)
                                {
                                    for (uint8_t n=m+1; n<=order; n++)
                                    {
                                        Field* fl = unit[l-1];
                                        Field* fm = unit[m-1];
                                        Field* fn = unit[n-1];
                                        if (*fl->getVal()==0 && *fm->getVal()==0 && *fn->getVal()==0)
                                        {
                                            bool flIsNakedTriple = true;
                                            for (uint8_t c : nonMatchCands)
                                            {
                                                if (std::find(fl->getCandidates()->begin(), fl->getCandidates()->end(), c) != fl->getCandidates()->end())
                                                {
                                                    flIsNakedTriple = false;
                                                }
                                            }
                                            bool fmIsNakedTriple = true;
                                            for (uint8_t c : nonMatchCands)
                                            {
                                                if (std::find(fm->getCandidates()->begin(), fm->getCandidates()->end(), c) != fm->getCandidates()->end())
                                                {
                                                    fmIsNakedTriple = false;
                                                }
                                            }
                                            bool fnIsNakedTriple = true;
                                            for (uint8_t c : nonMatchCands)
                                            {
                                                if (std::find(fn->getCandidates()->begin(), fn->getCandidates()->end(), c) != fn->getCandidates()->end())
                                                {
                                                    fnIsNakedTriple = false;
                                                }
                                            }
                                            if (flIsNakedTriple && fmIsNakedTriple && fnIsNakedTriple && numFreeFieldsInUnit > 3)
                                            {
                                                // this->_logTextArea->append("FOUND NAKED TRIPLE " + QString::fromStdString(type) + " " + QString::number(unitID) + " : " + QString::number(i) + " " + QString::number(j) + " " + QString::number(k));
                                                // this->_logTextArea->append("Fields " + QString::number(*fl->getRID()) + " " + QString::number(*fl->getCID()) + " , " + QString::number(*fm->getRID()) + " " + QString::number(*fm->getCID()) + " , " + QString::number(*fn->getRID()) + " " + QString::number(*fn->getCID()));
                                                NakedTriple* firstNakedTriple = new NakedTriple(fl,fm,fn, i,j,k, type);
                                                bool isDead = false;
                                                for (NakedTriple* dead : deadNakedTriples)
                                                {
                                                    if (dead->getField1() == firstNakedTriple->getField1() &&
                                                        dead->getField2() == firstNakedTriple->getField2() &&
                                                        dead->getField3() == firstNakedTriple->getField3() &&
                                                        dead->getType() == firstNakedTriple->getType())
                                                    {
                                                        isDead = true;
                                                        // this->_logTextArea->append("IS DEAD!");
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
        // this->_logTextArea->append("firstNakedTriple() == nullptr");
        return nullptr;
    }


    void Sudoku::eliminateCandidatesOfNakedTripleInUnit(NakedTriple* nakedTriple, std::array<Field*, order> unit)
    {
        for (Field* f : unit)
        {
            if (f->getFID() == nakedTriple->getField1()->getFID() ||
                f->getFID() == nakedTriple->getField2()->getFID() ||
                f->getFID() == nakedTriple->getField3()->getFID() ||
                *f->getVal() != 0)
                continue;
            std::vector<uint8_t>* candidates = f->getCandidates();
            std::vector<uint8_t>* candidatesToRemove = nakedTriple->getField1()->getCandidates();

            for (uint8_t const candidate : *candidatesToRemove)
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

    void Sudoku::processNakedTriples(uint8_t run)
    {
        this->_logTextArea->append("\nSearch for NakedTriples");
        std::vector<NakedTriple*> deadNakedTriples;
        while (true)
        {
            NakedTriple* firstNakedTriple= this->firstNakedTriple(deadNakedTriples);
            if (firstNakedTriple == nullptr)
            {
                break;
            }

            uint8_t const numCandsBeforeEliminatingFirstNakedTriple = this->countCandidates();
            this->eliminateNakedTriple(firstNakedTriple);

            if (this->countCandidates() < numCandsBeforeEliminatingFirstNakedTriple)
            {
                QString const msg =
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
    }

    HiddenSubset* Sudoku::firstHiddenTriple()
    {
        for (const std::string& type : {"Row", "Col", "Block"})
        {
            for (uint8_t unitID = 1; unitID <= order; unitID++) // go over all units
            {
                array<Field*, order> unit;
                if (type == "Row")
                    unit = getRowByRowID(unitID);
                else if (type == "Col")
                    unit = getColByColID(unitID);
                else if (type == "Block")
                    unit = getBlockByBlockID(unitID);
                uint8_t* candidateOccurrences = candidateOccurrencesInUnit(unit);
                for (uint8_t i = 1; i <= order - 2; i++) // Alle Zahlen-Tripel {1,2,3} bis {7,8,9}
                {
                    for (uint8_t j = i + 1; j <= order - 1; j++)
                    {
                        for (uint8_t k = j + 1; k <= order; k++)
                        {
                            if (candidateOccurrences[i - 1] > 0 && candidateOccurrences[j - 1] > 0 && candidateOccurrences[k - 1] > 0)
                            {
                                for (uint8_t l = 1; l <= order - 2; l++) // Alle Feld-Kombinationen in unit
                                {
                                    for (uint8_t m = l + 1; m <= order - 1; m++)
                                    {
                                        //                                    loop:
                                        for (uint8_t n = m + 1; n <= 9; n++)
                                        {
                                            // i,j,k are only allowed in fields l,m,n
                                            Field* fl = unit[l - 1];
                                            Field* fm = unit[m - 1];
                                            Field* fn = unit[n - 1];

                                            if ((*fl->getVal() != 0 || *fm->getVal() != 0 || *fn->getVal() != 0) ||
                                                (fl->getCandidates()->size() <= 3 && fm->getCandidates()->size() <= 3 && fn->getCandidates()->size() <= 3))
                                            {
                                                continue;
                                            }
                                            bool isHiddenTriple = true;
                                            for (Field* f : unit)
                                            {
                                                if (f != fl && f != fm && f != fn)
                                                {
                                                    if (std::find(f->getCandidates()->begin(), f->getCandidates()->end(), i) != f->getCandidates()->end() ||
                                                        std::find(f->getCandidates()->begin(), f->getCandidates()->end(), j) != f->getCandidates()->end() ||
                                                        std::find(f->getCandidates()->begin(), f->getCandidates()->end(), k) != f->getCandidates()->end())
                                                    {
                                                        isHiddenTriple = false;
                                                        //                                                        goto loop;
                                                    }
                                                }
                                            }
                                            if (isHiddenTriple)
                                            {
                                                return new HiddenSubset(std::vector<Field*>({fl, fm, fn}), std::vector<uint8_t>({i, j, k}), type);
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

    // Hidden Triple methods
    void Sudoku::processHiddenTriples(uint8_t run)
    {
        this->_logTextArea->append("\nSearch for HiddenTriples");
        while (true)
        {
            HiddenSubset* firstHiddenTriple = this->firstHiddenTriple();
            if (firstHiddenTriple == nullptr)
            {
                break;
            }
            int numCandsBeforeEliminatingHiddenTriple = this->countCandidates();
            std::vector<uint8_t> const hiddenTripleCanidates = firstHiddenTriple->getCandidates();
            for (Field* f : firstHiddenTriple->getFields())
                retainAll(*f->getCandidates(), hiddenTripleCanidates);
            if (this->countCandidates() < numCandsBeforeEliminatingHiddenTriple)
            {
                QString const msg = "HiddenTriple {" +
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
            // this.print();
            // this.printFields();
        }
    }

    // Main Solving routine
    void Sudoku::solve(const std::string& name)
    {
        this->_logTextArea->clear();
        this->_logTextArea->append("START SOLVING SUDOKU '" + QString::fromStdString(name) + "'");
        this->_logTextArea->append("Initial free fields: " + QString::number(this->getFreeFields().size()));
        this->_logTextArea->append("Initial candidates: " + QString::number(this->countCandidates()) + "\n");

        uint8_t run = 0;

        addStepToList(run, "Initial status");

        std::chrono::high_resolution_clock::time_point t1;
        std::chrono::high_resolution_clock::time_point const t0 = std::chrono::high_resolution_clock::now();

        // Main solving loop
        // while (this->getFreeFields().size() > 0)
        while (true)
        {
            run++;

            auto freeFieldsBeforeRun = this->getFreeFields().size();
            auto numCandsBeforeRun = this->countCandidates();

            this->_logTextArea->append("BEGIN Run No. " + QString::number(run) + ":");
            this->_logTextArea->append(QString::number(freeFieldsBeforeRun) + " free fields, " + QString::number(numCandsBeforeRun) + " candidates\n");

            // Process Singles
            this->processNakedSingles(run);  // Check Naked Singles
            this->processHiddenSingles(run); // Check Hidden Singles
            auto freeFieldsAfterSingles = this->getFreeFields().size();

            // If no (more) Singles exist -> Search for Naked and Hidden Pairs
            if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && this->getFreeFields().size() > 0)
            {
                if (freeFieldsAfterSingles < freeFieldsBeforeRun)
                {
                    this->_logTextArea->append("\nNo further Singles found.\nProceeding with Pairs...");
                }
                else
                {
                    this->_logTextArea->append("\nNo Singles found.\nProceeding with pairs...");
                }

                uint8_t const numCandsBeforePairs = this->countCandidates();
                this->processNakedPairs(run);  // Check Naked Pairs
                this->processHiddenPairs(run); // Check Hidden Pairs
                uint8_t const numCandsAfterPairs = this->countCandidates();

                // If Pair techniques didn't produce Singles -> search for Naked and Hidden Triples
                if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && this->getFreeFields().size() > 0)
                {
                    if (numCandsAfterPairs < numCandsBeforePairs)
                    {
                        this->_logTextArea->append("\nPair techniques didn't produce Singles.\nProceeding with Triples...");
                    }
                    else
                    {
                        this->_logTextArea->append("\nNo Pairs found.\nProceeding with Triples...");
                    }

                    uint8_t numCandsBeforeTriples = this->countCandidates();
                    this->processNakedTriples(run);  // Check Naked Triples
                    this->processHiddenTriples(run); // Check Hidden Triples
                    uint8_t numCandsAfterTriples = this->countCandidates();

                    // If pair/triple techniques didn't produce Singles -> go for Row-Block-Checks and Block-Row-Checks
                    if (this->firstNakedSingle() == nullptr && this->firstHiddenSingle() == nullptr && this->getFreeFields().size() > 0)
                    {
                        if (numCandsAfterTriples < numCandsBeforeTriples)
                        {
                            this->_logTextArea->append("\nTriple techniques didn't produce Singles.\nProceeding with LockedCandidates...");
                        }
                        else
                        {
                            this->_logTextArea->append("\nNo Triples found.\nProceeding with LockedCandidates...");
                        }

                        // int numCandsBeforeLockedCands = this.countCandidates();
                        // this.performRowBlockChecks(run, steps); // Peform Row-Block-Checks
                        // this.performBlockRowChecks(run, steps); // Perfom Block-Row-Checks
                        // int numCandsAfterLockedCands = this.countCandidates();
                    }
                    else
                    {
                        this->_logTextArea->append("\nTriple techniques produced Singles.\nProceeding with next run...");
                    }
                }
                else
                {
                    this->_logTextArea->append("\nPair techniques produced Singles.\nProceeding with next run...");
                }
            }
            else
            {
                if (this->getFreeFields().size() > 0)
                {
                    this->_logTextArea->append("\nNo more HiddenSingles, but new NakedSingles.\nProceeding with next run...");
                }
            }

            auto freeFieldsAfterRun = this->getFreeFields().size();
            auto numCandsAfterRun = this->countCandidates();

            this->_logTextArea->append("\nEND Run No. " + QString::number(run) + ":");
            this->_logTextArea->append(QString::number(freeFieldsAfterRun) + " free fields, " + QString::number(numCandsAfterRun) + " candidates\n‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒\n");

            if (numCandsAfterRun == 0)
            {
                t1 = std::chrono::high_resolution_clock::now();
                this->_logTextArea->append("Finished: No free fields remaining\n");
                break;
            }

            if (numCandsBeforeRun == numCandsAfterRun)
            {
                t1 = std::chrono::high_resolution_clock::now();
                this->_logTextArea->append("Abort: No more solving techniques implemented\n");
                break;
            }
        }

        if (this->getFreeFields().size() == 0)
        {
            this->_logTextArea->append("SUDOKU '" + QString::fromStdString(name) + "' SUCESSFULLY SOLVED!\n");
        }
        else
        {
            this->_logTextArea->append("SOLVING SUDOKU '" + QString::fromStdString(name) + "' FAILED!\n");
            this->_logTextArea->append("\nAbort status:");
            // this->print();
            // this->printFields();
            this->_logTextArea->append("\nAbort with " + QString::number(this->getFreeFields().size()) + " free fields and " + QString::number(this->countCandidates()) + " candidates\n");
        }

        this->_logTextArea->append("Elapsed time: " + QString::number(std::chrono::duration<double, std::milli>(t1 - t0).count(), 'f', 1) + " ms\n");
    }

    void Sudoku::addStepToList(uint8_t run, const QString& type)
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
        uint8_t i = 1;
        for (uint8_t rID = 1; rID <= order; ++rID)
        {
            QString str = " " + QString::number(i) + " |";
            uint8_t j = 1;
            for (uint8_t cID = 1; cID <= order; ++cID)
            {
                //const Field* f = &_grid[r - 1][c - 1];
                const uint8_t* val = _grid[rID - 1][cID - 1].getVal();
                if (*val != 0)
                {
                    str += " " + QString::number(*val) + " ";
                }
                else
                {
                    str += "   ";
                }
                if (j % 3 == 0 && j != order)
                {
                    str += "|";
                }
                j++;
            }
            printStream << str << "|" << Qt::endl;
            if (i % 3 == 0 && i != order)
            {
                printStream << "   |---------+---------+---------|" << Qt::endl;
            }
            i++;
        }
        printStream << "    ----------------------------- " << Qt::endl;
        printStream << Qt::endl;

        _logTextArea->append(printString);
    }

    void Sudoku::printFields() const
    {
        QString printString;
        QTextStream printStream(&printString);
        for (uint8_t rID = 1; rID <= order; rID++)
        {
            for (uint8_t cID = 1; cID <= order; cID++)
            {
                _grid[rID - 1][cID - 1].printField(*_logTextArea);
            }
        }
        printStream << Qt::endl;
        _logTextArea->append(printString);
    }

} // namespace sudoku
