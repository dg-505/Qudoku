cmake_minimum_required(VERSION 3.5)

# extract version number from InnoSetup script
file(READ "../util/createSetup.iss" ISS_CONTENT)
string(REGEX MATCH "Version \"([0-9]+\\.[0-9]+\\.[0-9]+)\"" _ ${ISS_CONTENT})
set(VERSION_NUMBER "${CMAKE_MATCH_1}")
message("Qudoku Version: ${VERSION_NUMBER}")
# Replace old version number with new version number
set(MAIN_CPP_FILE "./src/main.cpp")
file(READ ${MAIN_CPP_FILE} MAIN_CPP_CONTENTS)
string(REGEX REPLACE "\"([0-9]+\\.[0-9]+\\.[0-9]+)\";" "\"${VERSION_NUMBER}\";" MODIFIED_CONTENT "${MAIN_CPP_CONTENTS}")
file(WRITE ${MAIN_CPP_FILE} "${MODIFIED_CONTENT}")

project(Qudoku VERSION ${VERSION_NUMBER} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/res/Qudoku.rc")

add_executable(Qudoku
        src/main.cpp ${app_icon_resource_windows} include/globals.h
        src/gui/TechniquesDialog.cpp include/gui/CandidatesGUI.h include/gui/MainGUI.h include/gui/QInputField.h include/gui/QLogTextBrowser.h include/gui/SolvedGUI.h include/gui/StepByStepGUI.h
        include/sudoku/Field.h include/sudoku/HiddenSingle.h include/sudoku/HiddenSubset.h include/sudoku/NakedPair.h include/sudoku/NakedTriple.h src/sudoku/Step.cpp include/sudoku/Sudoku.h
        include/gui/TechniquesDialog.h src/gui/CandidatesGUI.cpp src/gui/MainGUI.cpp src/gui/QInputField.cpp src/gui/QLogTextBrowser.cpp src/gui/SolvedGUI.cpp src/gui/StepByStepGUI.cpp
        src/sudoku/Field.cpp src/sudoku/HiddenSingle.cpp src/sudoku/HiddenSubset.cpp src/sudoku/NakedPair.cpp src/sudoku/NakedTriple.cpp include/sudoku/Step.h src/sudoku/Sudoku.cpp
        resources.qrc
)

if (NOT WIN32)
    target_link_directories(Qudoku PRIVATE /opt/qudoku/lib)
endif ()
target_link_libraries(Qudoku PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Qudoku PROPERTIES WIN32_EXECUTABLE TRUE)
