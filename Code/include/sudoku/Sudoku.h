#pragma once

#include <array>
#include <string>
#include <vector>

#include "globals.h"
#include "gui/QLogTextBrowser.h"
#include "sudoku/Field.h"
#include "sudoku/HiddenSingle.h"
#include "sudoku/HiddenSubset.h"
#include "sudoku/NakedPair.h"
#include "sudoku/NakedTriple.h"
#include "sudoku/Step.h"

namespace sudoku
{
    class Sudoku
    {
        private:
            std::string _name;
            std::array<std::array<Field, global::order>, global::order> _grid;
            std::vector<Step> _steps;
            QLogTextBrowser* _logTextArea;

            bool _useNakedSingles;
            bool _useHiddenSingles;
            bool _useNakedPairs;
            bool _useHiddenPairs;
            bool _useNakedTriples;
            bool _useHiddenTriples;
            bool _useBlockLineChecks;
            bool _useLineBlockChecks;
            bool _useBacktracking;

        public:
            Sudoku(const std::string& name, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>* vals, QLogTextBrowser* logTextArea, bool nakedSinglesEnabled, bool hiddenSinglesEnabled, bool nakedPairsEnabled, bool hiddenPairsEnabled, bool nakedTriplesEnabled, bool hiddenTriplesEnabled, bool blockLineChecksEnabled, bool lineBlockChecksEnabled, bool backtrackingEnabled);

            Sudoku(const Sudoku&) = default;
            Sudoku(Sudoku&&) = delete;
            auto operator=(const Sudoku&) -> Sudoku& = delete;
            auto operator=(Sudoku&&) -> Sudoku& = delete;

            ~Sudoku() = default;

            auto getGrid() -> std::array<std::array<Field, global::order>, global::order>*;
            auto getSteps() -> std::vector<Step>*;
            // auto getLogTextArea() -> QLogTextBrowser*;

            // auto getFieldByCoord(uint8_t rID, uint8_t cID) -> Field*;
            // auto getFieldByFieldID(uint8_t fID) -> Field*;
            auto getRowByRowID(uint8_t rID) -> std::array<Field*, global::order>;
            auto getRowByFieldID(uint8_t fID) -> std::array<Field*, global::order>;
            auto getColByColID(uint8_t cID) -> std::array<Field*, global::order>;
            auto getColByFieldID(uint8_t fID) -> std::array<Field*, global::order>;
            auto getBlockByBlockID(uint8_t bID) -> std::array<Field*, global::order>;
            auto getBlockByFieldID(uint8_t fID) -> std::array<Field*, global::order>;

            auto getFreeFields() -> std::vector<Field*>;
            auto countCandidates() -> uint16_t;
            void filldAndEliminate(Field* field);

            auto firstNakedSingle() -> Field*;
            void processNakedSingles(uint8_t run);

            static auto fieldContainsCandidate(std::vector<uint8_t>* candidates, uint8_t cand) -> bool;
            static auto candidateOccurrencesInUnit(std::array<Field*, global::order> unit) -> std::array<uint8_t, global::order>;
            auto firstHiddenSingle() -> HiddenSingle*;
            void processHiddenSingles(uint8_t run);

            auto firstNakedPair(std::vector<NakedPair*>& deadNakedPairs) -> NakedPair*;
            static void eliminateCandidatesOfNakedPairInUnit(NakedPair* nakedPair, const std::array<Field*, global::order>& unit);
            void eliminateNakedPair(NakedPair* nakedPair);
            void processNakedPairs(uint8_t run);

            auto firstHiddenPair() -> HiddenSubset*;
            static void retainAll(std::vector<uint8_t>& candidates, const std::vector<uint8_t>& hiddenPairCandidates);
            void processHiddenPairs(uint8_t run);

            auto firstNakedTriple(std::vector<NakedTriple*>& deadNakedTriples) -> NakedTriple*;
            static void eliminateCandidatesOfNakedTripleInUnit(NakedTriple* nakedTriple, const std::array<Field*, global::order>& unit);
            void eliminateNakedTriple(NakedTriple* nakedTriple);
            void processNakedTriples(uint8_t run);

            auto firstHiddenTriple() -> HiddenSubset*;
            void processHiddenTriples(uint8_t run);

            static auto findFieldsInUnitContainingCandidateI(const std::array<Field*, global::order>& unit, uint8_t cand) -> std::vector<Field*>;
            void removeCandidateIFromUnit(uint8_t cand, const std::vector<Field*>& containingCandidateI, uint8_t unitIdOfFirst, const std::string& type);
            void eliminateBRCfromLine(uint8_t run, uint8_t blockID, uint8_t cand, const std::vector<Field*>& containingCandidateI, const std::string& type, uint8_t lineIdOfFirst, bool allCandidatesInSameLine);
            void performBlockRowChecks(uint8_t run);
            void performRowBlockChecks(uint8_t run);

            // Last resort: Try and error with backtracking
            static auto unitContainsVal(uint8_t val, const std::array<Field*, global::order>& unit) -> bool;
            auto backtracking() -> bool;

            void solve();

            void print() const;
            void printFields() const;
            // bool validateInput(QWidget* msgParent);

            auto getUnit(const std::string& type, uint8_t unitID) -> std::array<Field*, global::order>;

            static auto getUnitNumber(Field* field, const std::string& type) -> uint8_t;
    };
} // namespace sudoku
