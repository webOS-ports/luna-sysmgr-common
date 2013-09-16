project(LunaSysMgrCommon)

cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC TRUE)

include(webOS/webOS)
webos_modules_init(1 0 0 QUALIFIER RC3)
webos_component(3 0 0)

webos_add_compiler_flags(ALL -fno-rtti -fno-exceptions -Wall -fpermissive)
webos_add_compiler_flags(ALL -DFIX_FOR_QT)
webos_add_compiler_flags(ALL -Wno-unused-parameter -Wno-unused-variable -Wno-reorder -Wno-missing-field-initializers -Wno-extra)

find_package(Qt5Core REQUIRED)
if(NOT Qt5Core_FOUND)
    message(FATAL_ERROR "Qt5Core module is required!")
endif()

find_package(Qt5Gui REQUIRED)
if(NOT Qt5Gui_FOUND)
    message(FATAL_ERROR "Qt5Gui module is required!")
endif()

find_package(Qt5Widgets REQUIRED)
if(NOT Qt5Widgets_FOUND)
    message(FATAL_ERROR "Qt5Widgets module is required!")
endif()

find_package(PkgConfig "0.22" REQUIRED)

pkg_check_modules(GLIB2 glib-2.0 REQUIRED)
pkg_check_modules(GTHREAD2 gthread-2.0 REQUIRED)
pkg_check_modules(LS2 luna-service2 REQUIRED)
pkg_check_modules(LUNA_SYSMGR_IPC LunaSysMgrIpc REQUIRED)
pkg_check_modules(LUNA_SYSMGR_IPC_MESSAGES LunaSysMgrIpcMessages REQUIRED)
pkg_check_modules(LUNA_PREFS luna-prefs REQUIRED)
pkg_check_modules(NYX nyx REQUIRED)
pkg_check_modules(PBNJSON_CPP pbnjson_cpp REQUIRED)
pkg_check_modules(SQLITE3 sqlite3 REQUIRED)
pkg_check_modules(PMLOGLIB PmLogLib REQUIRED)

include_directories(
    ${GLIB2_INCLUDE_DIRS}
    ${GTHREAD2_INCLUDE_DIRS}
    ${LS2_INCLUDE_DIRS}
    ${LUNA_SYSMGR_IPC_INCLUDE_DIRS}
    ${LUNA_SYSMGR_IPC_MESSAGES_INCLUDE_DIRS}
    ${LUNA_PREFS_INCLUDE_DIRS}
    ${NYX_INCLUDE_DIRS}
    ${PBNJSON_CPP_INCLUDE_DIRS}
    ${SQLITE3_INCLUDE_DIRS}
    ${PMLOGLIB_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/Src/base
    ${CMAKE_SOURCE_DIR}/Src/base/application
    ${CMAKE_SOURCE_DIR}/Src/base/hosts
    ${CMAKE_SOURCE_DIR}/Src/base/settings
    ${CMAKE_SOURCE_DIR}/Src/core
    ${CMAKE_SOURCE_DIR}/Src/nyx
    ${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    Src/base/settings/Settings.cpp
    Src/base/settings/LocalePreferences.cpp
    Src/base/settings/AnimationSettings.cpp
    Src/base/hosts/HostBase.cpp
    Src/base/hosts/HostArm.cpp
    Src/base/hosts/HostUnknown.h
    Src/base/JSONUtils.cpp
    Src/base/Utils.cpp
    Src/base/WindowProperties.cpp
    Src/base/Localization.cpp
    Src/base/Logging.cpp
    Src/base/application/ApplicationDescriptionBase.cpp
    Src/core/SyncTask.cpp
    Src/core/Timer.cpp
    Src/core/SingletonTimer.cpp
    Src/core/TaskBase.cpp
    Src/core/KeywordMap.cpp
    Src/core/Mutex.cpp
    Src/core/AsyncCaller.cpp
    Src/nyx/NyxLedControl.cpp
    Src/nyx/NyxInputControl.cpp
    Src/nyx/NyxSensorConnector.cpp
    include/ProcessKiller.h
    include/LocalePreferences.h
    include/HostBase.h
    include/NyxSensorConnector.h)

add_library(LunaSysMgrCommon SHARED ${SOURCES})
target_link_libraries(LunaSysMgrCommon
    ${LS2_LIBRARIES}
    ${GLIB2_LIBRARIES}
    ${GTHREAD2_LIBRARIES}
    ${LUNA_SYSMGR_IPC_LIBRARIES}
    ${LUNA_PREFS_LIBRARIES}
    ${NYX_LIBRARIES}
    ${PBNJSON_CPP_LIBRARIES}
    ${SQLITE3_LIBRARIES}
    ${PMLOGLIB_LIBRARIES}
    pthread)
qt5_use_modules(LunaSysMgrCommon Core Gui Widgets)

install(DIRECTORY include/ DESTINATION ${WEBOS_INSTALL_INCLUDEDIR}/sysmgr-common FILES_MATCHING PATTERN "*.h")

webos_build_pkgconfig()
webos_build_library(NOHEADERS)
