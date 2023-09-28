#pragma once

//#include <QMessageBox>
// #include <QtWidgets/QPlainTextEdit>
#include <array>
#include <vector>

#include "globals.h"
#include "gui/QLogTextBrowser.h"
#include "sudoku/Field.h"
#include "sudoku/HiddenSingle.h"
#include "sudoku/HiddenSubset.h"
#include "sudoku/NakedPair.h"
#include "sudoku/NakedTriple.h"

using std::array;
using std::vector;

namespace sudoku
{
    class Sudoku
    {
        private:
            array<array<Field, order>, order> _grid;
            vector<array<array<Field, order>, order>> _steps;
            QLogTextBrowser* _logTextArea;
            vector<uint8_t> _foundInRunNo;
            vector<QString> _foundByType;

        public:
            //Sudoku();
            Sudoku(const uint8_t* vals, QLogTextBrowser& logTextArea);
            //Sudoku(const uint8_t start[][3]);
            ~Sudoku() = default;

            //Field* createEmptyGrid();
            // Field** getArray();
            // void setArray(Field** arr);
            vector<array<array<Field, order>, order>>* getSteps();
            // void setSteps(std::vector<Field**> s);
            QLogTextBrowser* getLogTextArea();
            // void setLogTextArea(QPlainTextEdit* ta);
            vector<uint8_t>* getFoundInRunNo();
            // void setFoundInRunNo(const std::vector<int>& n);
            vector<QString>* getFoundByType();
            // void setFoundByType(const std::vector<QString>& s);

            Field* getFieldByCoord(const uint8_t& rID, const uint8_t& cID);
            Field* getFieldByFieldID(const uint8_t& fID);
            array<Field*, order> getRowByRowID(const uint8_t& rID);
            array<Field*, order> getRowByFieldID(const uint8_t& fID);
            array<Field*, order> getColByColID(const uint8_t& cID);
            array<Field*, order> getColByFieldID(const uint8_t& fID);
            //Field** getBlockByUpperLeftField(uint8_t r0, uint8_t c0) const;
            array<Field*, order> getBlockByBlockID(const uint8_t& bID);
            array<Field*, order> getBlockByFieldID(const uint8_t& fID);

            std::vector<Field*> getFreeFields();
            uint8_t countCandidates();
            // std::vector<int> findCandidatesForField(Field* f);
            void filldAndEliminate(Field* field);

            Field* firstNakedSingle();
            void processNakedSingles(uint8_t run);

            bool fieldContainsCandidate(std::vector<uint8_t>* candidates, uint8_t cand);
            array<uint8_t, order> candidateOccurrencesInUnit(array<Field*, order> unit);
            HiddenSingle* firstHiddenSingle();
            void processHiddenSingles(uint8_t run);

            // bool containsAll(const std::vector<int>& candidates, const std::vector<int>& c);
            NakedPair* firstNakedPair(std::vector<NakedPair*>& deadNakedPairs);
            void eliminateCandidatesOfNakedPairInUnit(NakedPair* nakedPair, array<Field*, order> unit);
            void eliminateNakedPair(NakedPair* nakedPair);
            void processNakedPairs(uint8_t run);

            HiddenSubset* firstHiddenPair();
            void retainAll(std::vector<uint8_t>& candidates, const std::vector<uint8_t>& hiddenPairCandidates);
            void processHiddenPairs(uint8_t run);

            NakedTriple* firstNakedTriple(std::vector<NakedTriple*>& deadNakedTriples);
            void eliminateCandidatesOfNakedTripleInUnit(NakedTriple* nakedTriple, array<Field*, order> unit);
            void eliminateNakedTriple(NakedTriple* nakedTriple);
            void processNakedTriples(uint8_t run);

            HiddenSubset* firstHiddenTriple();
            void processHiddenTriples(uint8_t run);

            static std::vector<Field*> findFieldsInUnitContainingCandidateI(const array<Field*, order>& unit, uint8_t cand);
            void removeCandidateIFromUnit(uint8_t cand, const std::vector<Field*>& containingCandidateI, uint8_t unitIdOfFirst, const std::string& type);
            void eliminateBRCfromLine(const uint8_t& run, uint8_t blockID, uint8_t cand, const std::vector<Field*>& containingCandidateI, const std::string& type, uint8_t lineIdOfFirst, bool allCandidatesInSameLine);
            void performBlockRowChecks(uint8_t run);
            void performRowBlockChecks(uint8_t run);

            void solve(const std::string& name);

            void addStepToList(uint8_t run, const QString& type);
            void print() const;
            void printFields() const;
            // Field* toArray1D(Field** grid2D);
            // bool validateInput(QWidget* msgParent);
    };
} // namespace sudoku
