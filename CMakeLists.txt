cmake_minimum_required(VERSION 3.5)

project(lubuntu-update VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES translations/lubuntu-update_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        orchestrator.h
        orchestrator.cpp
        aptmanager.h
        aptmanager.cpp
        conffilewidget.h
        conffilewidget.cpp
        conffilewidget.ui
        diffdisplaydialog.h
        diffdisplaydialog.cpp
        diffdisplaydialog.ui
        conffilehandlerdialog.h
        conffilehandlerdialog.cpp
        conffilehandlerdialog.ui
        ipcfilewatcher.h
        ipcfilewatcher.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(lubuntu-update
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(lubuntu-update
        ${PROJECT_SOURCES}
        resources.qrc
    )

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(lubuntu-update PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS lubuntu-update
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Yes, we are hardcoding a path here. Yes, it's ugly. However, the program
# will attempt to run this script as root, and I do NOT want to make it try to
# "find" the script and execute the first thing it finds that has the right
# name, that sounds like a security breach waiting to happen. It will only
# execute lubuntu-update-backend from /usr/libexec, and if something malicious
# has gotten into /usr/libexec you're compromised anyway, so not much to worry
# about. Therefore we always, *always* install this script into /usr/libexec/,
# no matter where the rest of the program goes.
install(FILES lubuntu-update-backend DESTINATION /usr/libexec/)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(lubuntu-update)
endif()
