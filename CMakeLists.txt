cmake_minimum_required(VERSION 3.19)

project(FinanceApp LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Widgets
    Charts
    Network
    TextToSpeech
    LinguistTools
)

qt_standard_project_setup()

qt_add_executable(FinanceApp
    WIN32
    MACOSX_BUNDLE

    main.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    chartdialog.cpp
    chartdialog.h
)

qt_add_translations(
    TARGETS FinanceApp
    TS_FILES FinanceApp_id_ID.ts
)

target_link_libraries(FinanceApp
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Charts
        Qt6::Network
        Qt6::TextToSpeech
)

install(TARGETS FinanceApp
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET FinanceApp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})