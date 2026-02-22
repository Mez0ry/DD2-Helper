cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME DD2-Helper)

project(${TARGET_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(UI_DIRECTORY "${CMAKE_SOURCE_DIR}/ui")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui  Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES DD2-Helper_en_001.ts)

set(PROJECT_SOURCES
    src/main.cpp src/mainwindow.cpp
    include/mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${TARGET_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        resource.qrc
        resources/app_styles/dark_theme.qss
        include/HotKeyEdit.hpp
        src/HotKeyEdit.cpp

        resources/icons/dark/find_windows.svg
        resources/icons/dark/settings.svg
        resources/icons/dark/find_windows.svg resources/icons/dark/settings.svg
        include/ThemeToggle.hpp
        src/ThemeToggle.cpp
        include/Process.hpp
        src/Process.cpp
        include/HandleGuard.hpp
        include/Window.hpp
        src/Window.cpp
        include/Player.hpp
        src/Player.cpp
        include/Utils.hpp
        src/Utils.cpp
        include/PlayerWidget.hpp
        src/PlayerWidget.cpp
        include/HotKeySequence.hpp
        src/HotKeySequence.cpp
        include/KeyboardInput.hpp
        src/KeyboardInput.cpp
        include/VKeys.hpp
        include/MouseInput.hpp
        src/MouseInput.cpp
        include/UserConfig.hpp
        src/UserConfig.cpp
        resources/app_styles/dark_theme.qss resources/app_styles/light_theme.qss resources/icons/dark/check_mark.svg resources/icons/dark/find_windows.svg resources/icons/dark/settings.svg resources/icons/generic/dark_mode.svg resources/icons/generic/light_mode.svg resources/user/user.json
        resources/user/user.json
        include/Commands/ICommand.hpp
        include/Commands/SendHotkey.hpp
        src/Commands/SendHotkey.cpp


        ui/mainwindow.ui
        ui/mainwindow.ui
        include/Popup.hpp

        src/Popup.cpp
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(${TARGET_NAME} SHARED
            ${PROJECT_SOURCES}
        )

    else()
        add_executable(${TARGET_NAME}
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

qt_add_ui(${TARGET_NAME} SOURCES ${UI_DIRECTORY}/mainwindow.ui)
target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" ${TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/Commands" "${CMAKE_CURRENT_SOURCE_DIR}/ui")
target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_forms})

add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/user $<TARGET_FILE_DIR:${TARGET_NAME}>/resources)


if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${TARGET_NAME})
endif()
set_target_properties(${TARGET_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${TARGET_NAME})
endif()
