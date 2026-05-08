cmake_minimum_required(VERSION 3.19)

project(FinanceApp LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Charts
    Network
    TextToSpeech
    LinguistTools
)

qt_standard_project_setup()

qt_add_executable(FinanceApp
    main.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    logindialog.cpp
    logindialog.h
    logindialog.ui

    registerdialog.cpp
    registerdialog.h
    registerdialog.ui

    chartdialog.cpp
    chartdialog.h

    speech.py
)

qt_add_translations(
    TARGETS FinanceApp
    TS_FILES FinanceApp_id_ID.ts
)

target_link_libraries(FinanceApp PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
    Qt6::Network
    Qt6::TextToSpeech
)

include(GNUInstallDirs)

install(TARGETS FinanceApp
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)