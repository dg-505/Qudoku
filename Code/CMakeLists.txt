cmake_minimum_required(VERSION 3.5)

project(Qudoku VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/res/Qudoku.rc")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Qudoku
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/main.cpp ${app_icon_resource_windows} include/globals.h
        src/gui/TechniquesDialog.cpp include/gui/CandidatesGUI.h include/gui/MainGUI.h include/gui/QInputField.h include/gui/QLogTextBrowser.h include/gui/SolvedGUI.h include/gui/StepByStepGUI.h
        include/sudoku/Field.h include/sudoku/HiddenSingle.h include/sudoku/HiddenSubset.h include/sudoku/NakedPair.h include/sudoku/NakedTriple.h src/sudoku/Step.cpp include/sudoku/Sudoku.h
        include/gui/TechniquesDialog.h src/gui/CandidatesGUI.cpp src/gui/MainGUI.cpp src/gui/QInputField.cpp src/gui/QLogTextBrowser.cpp src/gui/SolvedGUI.cpp src/gui/StepByStepGUI.cpp
        src/sudoku/Field.cpp src/sudoku/HiddenSingle.cpp src/sudoku/HiddenSubset.cpp src/sudoku/NakedPair.cpp src/sudoku/NakedTriple.cpp include/sudoku/Step.h src/sudoku/Sudoku.cpp
        resources.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Qudoku APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Qudoku SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Qudoku
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Qudoku PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Qudoku)
endif()
set_target_properties(Qudoku PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Qudoku
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Qudoku)
endif()
