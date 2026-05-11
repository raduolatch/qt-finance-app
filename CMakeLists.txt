cmake_minimum_required(VERSION 3.19)
project(FinanceApp LANGUAGES CXX)
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Charts
    Network
    TextToSpeech
    Sql
    LinguistTools
)
qt_standard_project_setup()
qt_add_executable(FinanceApp
    WIN32
    MACOSX_BUNDLE
    # MAIN
    main.cpp
    # MAIN WINDOW
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    # LOGIN
    logindialog.cpp
    logindialog.h
    logindialog.ui
    # REGISTER
    registerdialog.cpp
    registerdialog.h
    registerdialog.ui
    # CHART
    chartdialog.cpp
    chartdialog.h
    # PYTHON VOICE
    speech.py
    database.h
    database.cpp
    transaction.h
    admindialog.h
    admindialog.cpp
    admindialog.ui

)

# Copy speech.py ke folder build otomatis setiap kali build
configure_file(speech.py ${CMAKE_CURRENT_BINARY_DIR}/speech.py COPYONLY)

# TRANSLATION
qt_add_translations(
    TARGETS FinanceApp
    TS_FILES FinanceApp_id_ID.ts
)
# LINK LIBRARIES
target_link_libraries(FinanceApp PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
    Qt6::Network
    Qt6::TextToSpeech
    Qt6::Sql
)
# INSTALL
include(GNUInstallDirs)
install(TARGETS FinanceApp
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
# DEPLOY QT
qt_generate_deploy_app_script(
    TARGET FinanceApp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})