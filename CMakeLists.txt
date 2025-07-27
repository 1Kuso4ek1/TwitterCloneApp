cmake_minimum_required(VERSION 3.16)

project(TwitterCloneApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick NetworkAuth)

qt_standard_project_setup(REQUIRES 6.9.1)

qt_add_executable(appTwitterCloneApp
    src/main.cpp
    src/AuthManager.cpp
    include/AuthManager.hpp
    include/Config.hpp
)

target_include_directories(appTwitterCloneApp PUBLIC include/)

qt_add_qml_module(appTwitterCloneApp
    URI TwitterCloneApp
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/Feed.qml
        qml/Login.qml
        qml/Navigation.qml
)

qt_add_resources(appTwitterCloneApp
    "config"
    PREFIX
        "/"
    FILES
        "config/config.json"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appTwitterCloneApp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appTwitterCloneApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appTwitterCloneApp
    PRIVATE Qt6::Quick Qt6::NetworkAuth
)

include(GNUInstallDirs)
install(TARGETS appTwitterCloneApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
