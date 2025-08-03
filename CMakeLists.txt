cmake_minimum_required(VERSION 3.16)

project(TwitterCloneApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT EMSCRIPTEN)
    find_package(Qt6 REQUIRED COMPONENTS Quick NetworkAuth)
else()
    find_package(Qt6 REQUIRED COMPONENTS Quick Widgets)
endif()

set(
    APPLICATION_HEADERS

    include/Api/Api.hpp
    include/Api/AuthApi.hpp
    include/Api/PostsApi.hpp
    include/Api/UsersApi.hpp

    include/Auth/AuthManager.hpp

    include/Utils/Config.hpp
    include/Utils/PKCEUtils.hpp
    include/Utils/RequestHandler.hpp
    include/Utils/TokenStorage.hpp
)

set(
    APPLICATION_SOURCES

    src/Api/Api.cpp
    src/Api/AuthApi.cpp
    src/Api/PostsApi.cpp
    src/Api/UsersApi.cpp

    src/Utils/RequestHandler.cpp
    src/Utils/TokenStorage.cpp

    src/main.cpp
)

if(NOT EMSCRIPTEN)
    list(APPEND APPLICATION_HEADERS include/Auth/AuthManagerNative.hpp)
    list(APPEND APPLICATION_SOURCES src/Auth/AuthManagerNative.cpp)
else()
    list(APPEND APPLICATION_HEADERS include/Auth/AuthManagerWASM.hpp)
    list(APPEND APPLICATION_SOURCES src/Auth/AuthManagerWASM.cpp)

    if(ANDROID)
        set(ANDROID_MANIFEST android/AndroidManifest.xml)
    endif()
endif()

qt_standard_project_setup(REQUIRES 6.9.1)

qt_add_executable(TwitterClone
    MANUAL_FINALIZATION

    ${APPLICATION_HEADERS}
    ${APPLICATION_SOURCES}

    ${ANDROID_MANIFEST}
)

if(ANDROID)
    include(libraries/android_openssl/android_openssl.cmake)

    add_android_openssl_libraries(TwitterClone)
endif()


target_include_directories(TwitterClone PUBLIC include/ include/Api)

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
        qml/EditProfile.qml
        qml/FormField.qml
)

if(NOT EMSCRIPTEN)
    set(CONFIG_FILE "config/config.json")
else()
    set(CONFIG_FILE "config/config-wasm.json")
endif()

qt_add_resources(TwitterClone
    "config"
    PREFIX
        "/"
    FILES
        ${CONFIG_FILE}
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

if(NOT EMSCRIPTEN)
    target_link_libraries(TwitterClone
        PRIVATE Qt6::Quick Qt6::NetworkAuth
    )
else()
    target_link_libraries(TwitterClone
        PRIVATE Qt6::Quick Qt6::Widgets
    )
endif()

qt_finalize_executable(TwitterClone)

include(GNUInstallDirs)
install(TARGETS TwitterClone
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
