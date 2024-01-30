cmake_minimum_required(VERSION 3.5)

project(lubuntu-update VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES
	src/translations/lubuntu-update_en_US.ts
	src/translations/lubuntu-update_es_ES.ts
	src/translations/lubuntu-update_zh_CN.ts
)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/orchestrator.h
        src/orchestrator.cpp
        src/aptmanager.h
        src/aptmanager.cpp
        src/conffilewidget.h
        src/conffilewidget.cpp
        src/conffilewidget.ui
        src/diffdisplaydialog.h
        src/diffdisplaydialog.cpp
        src/diffdisplaydialog.ui
        src/conffilehandlerdialog.h
        src/conffilehandlerdialog.cpp
        src/conffilehandlerdialog.ui
        src/ipcfilewatcher.h
        src/ipcfilewatcher.cpp
        src/releaseupgradewindow.h
        src/releaseupgradewindow.cpp
        src/releaseupgradewindow.ui
        ${TS_FILES}
)

set(TRANSLATION_RESOURCES "src/translations.qrc")

configure_file(${TRANSLATION_RESOURCES} translations.qrc COPYONLY)
qt5_add_translation(QM_FILES ${TS_FILES})
qt5_add_resources(QM_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc)

add_custom_target(translations ALL DEPENDS ${QM_FILES})

add_executable(lubuntu-update
    ${PROJECT_SOURCES}
    ${QM_RESOURCES}
    src/resources.qrc
)

add_dependencies(lubuntu-update translations)

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
install(FILES src/lubuntu-update-backend DESTINATION /usr/libexec/)

install(FILES lubuntu-update.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/)
