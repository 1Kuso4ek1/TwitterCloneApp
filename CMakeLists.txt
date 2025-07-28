cmake_minimum_required(VERSION 3.16)

project(TwitterCloneApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick NetworkAuth)

set(
    APPLICATION_HEADERS

    include/AuthManager.hpp
    include/Config.hpp
    include/TokenStorage.hpp
    include/Api.hpp
)

set(
    APPLICATION_SOURCES

    src/AuthManager.cpp
    src/TokenStorage.cpp
    src/Api.cpp
    src/main.cpp
)

qt_standard_project_setup(REQUIRES 6.9.1)

qt_add_executable(TwitterClone
    MANUAL_FINALIZATION

    ${APPLICATION_SOURCES}
    ${APPLICATION_HEADERS}

    android/AndroidManifest.xml
)

if(ANDROID)
    include(libraries/android_openssl/android_openssl.cmake)

    add_android_openssl_libraries(TwitterClone)
endif()


target_include_directories(TwitterClone PUBLIC include/)

set_source_files_properties(qml/Navigation.qml
    PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(TwitterClone
    URI TwitterCloneApp
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/Feed.qml
        qml/Login.qml
        qml/Navigation.qml
        qml/Avatar.qml
        qml/Post.qml
        qml/NewPost.qml
        qml/Profile.qml
)

qt_add_resources(TwitterClone
    "config"
    PREFIX
        "/"
    FILES
        "config/config.json"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(TwitterClone PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TwitterClone
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

target_link_libraries(TwitterClone
    PRIVATE Qt6::Quick Qt6::NetworkAuth
)

qt_finalize_executable(TwitterClone)

include(GNUInstallDirs)
install(TARGETS TwitterClone
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
