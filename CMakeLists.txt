cmake_minimum_required(VERSION 3.25)

project(MControlCenter VERSION 0.5.2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS
    Widgets
    LinguistTools
    DBus
)

qt_standard_project_setup()

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.ui
    src/mainwindow.cpp src/mainwindow.h
    src/operate.cpp src/operate.h
    src/powermonitor.cpp src/powermonitor.h
    src/helper.cpp src/helper.h
    src/msi-ec_helper.cpp src/msi-ec_helper.h
    src/settings.cpp src/settings.h
    src/resources.qrc
)

#TODO: use I18N_SOURCE_LANGUAGE in qt_standard_project_setup() for translation
#when buntu/mint ship qt6.8
set(TS_FILES
    src/i18n/MControlCenter_de_DE.ts
    src/i18n/MControlCenter_en.ts
    src/i18n/MControlCenter_es.ts
    src/i18n/MControlCenter_eu.ts
    src/i18n/MControlCenter_fi_FI.ts
    src/i18n/MControlCenter_fr.ts
    src/i18n/MControlCenter_hu.ts
    src/i18n/MControlCenter_it.ts
    src/i18n/MControlCenter_nb.ts
    src/i18n/MControlCenter_nl_NL.ts
    src/i18n/MControlCenter_pt_BR.ts
    src/i18n/MControlCenter_pt_PT.ts
    src/i18n/MControlCenter_ru.ts
    src/i18n/MControlCenter_tr.ts
    src/i18n/MControlCenter_vi.ts
    src/i18n/MControlCenter_zh_CN.ts
)

qt_add_executable(mcontrolcenter
    ${PROJECT_SOURCES}
)

qt_add_translations(mcontrolcenter
    TS_FILES ${TS_FILES}
    RESOURCE_PREFIX "/translations"
    LUPDATE_OPTIONS
        -no-obsolete
        -locations none
)

add_subdirectory(src/helper ${CMAKE_BINARY_DIR}/helper)

target_link_libraries(mcontrolcenter PRIVATE
    Qt6::Widgets
    Qt6::DBus
)

target_compile_definitions(mcontrolcenter PRIVATE
    MControlCenter_VERSION="${PROJECT_VERSION}")
