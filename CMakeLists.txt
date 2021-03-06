#
#   QQmlModel CMake script by Olivier Le Doeuff

#
## CMAKE INPUT
#
#   - QQMLMODEL_TARGET : Name of the library target. Default : "QQmlModel"
#   - QQMLMODEL_PROJECT : Name of the project. Default : "QQmlModel"
#   - QQMLMODEL_BUILD_SHARED : Build shared library [ON OFF]. Default: OFF.
#   - QQMLMODEL_BUILD_STATIC : Build static library [ON OFF]. Default: ON.
#   - QQMLMODEL_USE_NAMESPACE : If the library compile with a namespace. Default: OFF.
#   - QQMLMODEL_NAMESPACE : Namespace for the library. Only relevant if QQMLMODEL_USE_NAMESPACE is ON. Default: "Qqm".
#   - QQMLMODEL_BUILD_DOC : Build the QQmlModel Doc [ON OFF]. Default: OFF.
#   - QQMLMODEL_DOXYGEN_BT_REPOSITORY : Repository of DoxygenBt. Default : "https://github.com/OlivierLDff/DoxygenBootstrapped.git"
#   - QQMLMODEL_DOXYGEN_BT_TAG : Git Tag of DoxygenBt. Default : "v1.3.0"

#
## CMAKE OUTPUT
#
#   - QQMLMODEL_TARGET : Output target to link to. Default: QQmlModel
#

#
## VERSION
#   - 1.0.0 : 05/12/2018 : Initial Work
#   - 1.0.1 : Fix DoxygenBootstrapped github link
#   - 1.0.4 : Fix CMakeLists filename
#.  - 1.2.0 : QSharedPointer<QObject> list

# ┌──────────────────────────────────────────────────────────────────┐
# │                       CMAKE PROPERTIES                           │
# └──────────────────────────────────────────────────────────────────┘

CMAKE_MINIMUM_REQUIRED( VERSION 3.1.0 )

# ┌──────────────────────────────────────────────────────────────────┐
# │                       PROJECT SETTINGS                           │
# └──────────────────────────────────────────────────────────────────┘


message(WARNING "This library is DEPRECATED, you should move to ObjectListModel (https://github.com/OlivierLDff/ObjectListModel)")

SET( QQMLMODEL_TARGET "QQmlModel" CACHE STRING "Target Name" )
SET( QQMLMODEL_PROJECT "QQmlModel" CACHE STRING "Project Name")
SET( QQMLMODEL_BUILD_SHARED OFF CACHE BOOL "Build shared lib for QQmlModel" )
IF(NOT QQMLMODEL_BUILD_SHARED)
SET( QQMLMODEL_BUILD_STATIC ON CACHE BOOL "Build static lib for QQmlModel" )
ELSE(NOT QQMLMODEL_BUILD_SHARED)
SET( QQMLMODEL_BUILD_STATIC OFF CACHE BOOL "Build static lib for QQmlModel" )
ENDIF(NOT QQMLMODEL_BUILD_SHARED)
SET( QQMLMODEL_USE_NAMESPACE ON CACHE BOOL "If the library compile with a namespace.")
SET( QQMLMODEL_NAMESPACE "Qqm" CACHE STRING "Namespace for the library. Only relevant if QQMLMODEL_USE_NAMESPACE is ON")
SET( QQMLMODEL_BUILD_DOC OFF CACHE BOOL "Build QQmlModel Doc with Doxygen" )
IF(QQMLMODEL_BUILD_DOC)
SET( QQMLMODEL_DOXYGEN_BT_REPOSITORY "https://github.com/OlivierLDff/DoxygenBootstrappedCMake.git" CACHE STRING "Repository of DoxygenBt" )
SET( QQMLMODEL_DOXYGEN_BT_TAG v1.3.2 CACHE STRING "Git Tag of DoxygenBt" )
ENDIF(QQMLMODEL_BUILD_DOC)

PROJECT( ${QQMLMODEL_PROJECT} )
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# ┌──────────────────────────────────────────────────────────────────┐
# │                         QT5 CMAKE                                │
# └──────────────────────────────────────────────────────────────────┘

# Find includes in corresponding build directories
SET( CMAKE_INCLUDE_CURRENT_DIR ON )
# Instruct CMake to run moc automatically when needed
SET( CMAKE_AUTOMOC ON )
 # https://cmake.org/cmake/help/v3.0/prop_tgt/AUTORCC.html#prop_tgt:AUTORCC
SET( CMAKE_AUTORCC ON )
#required by Qt5
set (CMAKE_CXX_STANDARD 11)

# Find correct qt directory
IF( QT_DIR )
    SET( CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${QT_DIR}" )
ENDIF( QT_DIR )

FIND_PACKAGE(Qt5Core CONFIG REQUIRED CMAKE_FIND_ROOT_PATH_BOTH)
FIND_PACKAGE(Qt5Qml CONFIG REQUIRED CMAKE_FIND_ROOT_PATH_BOTH)

# ┌──────────────────────────────────────────────────────────────────┐
# │                       VERSION                                    │
# └──────────────────────────────────────────────────────────────────┘

# Project version must be set here
SET(QQMLMODEL_VERSION_MAJOR 1)
SET(QQMLMODEL_VERSION_MINOR 2)
SET(QQMLMODEL_VERSION_PATCH 0)
SET(QQMLMODEL_VERSION ${QQMLMODEL_VERSION_MAJOR}.${QQMLMODEL_VERSION_MINOR}.${QQMLMODEL_VERSION_PATCH} CACHE STRING "Version")

IF( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git" )
    EXECUTE_PROCESS(
        COMMAND git describe --always
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE QQMLMODEL_VERSION_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
ELSE()
    SET( QQMLMODEL_VERSION_TAG 00000000 )
ENDIF()
SET( QQMLMODEL_VERSION_TAG_HEX 0x${QQMLMODEL_VERSION_TAG})

# ┌──────────────────────────────────────────────────────────────────┐
# │                       STATUS                                     │
# └──────────────────────────────────────────────────────────────────┘

MESSAGE( STATUS  "------ ${QQMLMODEL_TARGET} Configuration ${QQMLMODEL_VERSION} ------" )

MESSAGE( STATUS "QQMLMODEL_TARGET                 : ${QQMLMODEL_TARGET}" )
MESSAGE( STATUS "QQMLMODEL_PROJECT                : ${QQMLMODEL_PROJECT}" )
MESSAGE( STATUS "QQMLMODEL_VERSION                : ${QQMLMODEL_VERSION}" )
MESSAGE( STATUS "QQMLMODEL_VERSION_TAG_HEX        : ${QQMLMODEL_VERSION_TAG_HEX}" )
MESSAGE( STATUS "QQMLMODEL_BUILD_SHARED           : ${QQMLMODEL_BUILD_SHARED}" )
MESSAGE( STATUS "QQMLMODEL_BUILD_STATIC           : ${QQMLMODEL_BUILD_STATIC}" )
MESSAGE( STATUS "QQMLMODEL_USE_NAMESPACE          : ${QQMLMODEL_USE_NAMESPACE}" )
IF(QQMLMODEL_USE_NAMESPACE)
MESSAGE( STATUS "QQMLMODEL_NAMESPACE              : ${QQMLMODEL_NAMESPACE}" )
ENDIF(QQMLMODEL_USE_NAMESPACE)
MESSAGE( STATUS "QQMLMODEL_BUILD_DOC              : ${QQMLMODEL_BUILD_DOC}" )
IF(QQMLMODEL_BUILD_DOC)
MESSAGE( STATUS "QQMLMODEL_DOXYGEN_BT_REPOSITORY  : ${QQMLMODEL_DOXYGEN_BT_REPOSITORY}" )
MESSAGE( STATUS "QQMLMODEL_DOXYGEN_BT_TAG         : ${QQMLMODEL_DOXYGEN_BT_TAG}" )
ENDIF(QQMLMODEL_BUILD_DOC)

MESSAGE( STATUS "------ ${QQMLMODEL_TARGET} End Configuration ------" )

IF((QQMLMODEL_BUILD_SHARED AND QQMLMODEL_BUILD_STATIC) OR (NOT QQMLMODEL_BUILD_SHARED AND NOT QQMLMODEL_BUILD_STATIC))
MESSAGE( FATAL_ERROR "Set either Build shared or build static to ON, can't build both at once" )
ENDIF((QQMLMODEL_BUILD_SHARED AND QQMLMODEL_BUILD_STATIC) OR (NOT QQMLMODEL_BUILD_SHARED AND NOT QQMLMODEL_BUILD_STATIC))

# ┌──────────────────────────────────────────────────────────────────┐
# │                       SOURCES                                    │
# └──────────────────────────────────────────────────────────────────┘

SET( QQMLMODEL_SRCS
    # Main
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlObjectListModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlSharedObjectListModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlVariantListModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlVariantListModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlModelShared.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlModelShared.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlObjectListModel
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlVariantListModel
    )

# ┌──────────────────────────────────────────────────────────────────┐
# │                       TARGET                                     │
# └──────────────────────────────────────────────────────────────────┘

IF(QQMLMODEL_BUILD_SHARED)
    IF( MSVC )
    INCLUDE (GenerateExportHeader)
    ENDIF( MSVC )

    ADD_LIBRARY( ${QQMLMODEL_TARGET} SHARED ${QQMLMODEL_SRCS} )

    IF( MSVC )
    GENERATE_EXPORT_HEADER( ${QQMLMODEL_TARGET}
        BASE_NAME ${QQMLMODEL_TARGET}
        EXPORT_MACRO_NAME ${QQMLMODEL_TARGET}_EXPORT
        EXPORT_FILE_NAME ${QQMLMODEL_TARGET}_Export.h
        STATIC_DEFINE ${QQMLMODEL_TARGET}_BUILT_AS_STATIC
    )
    ENDIF( MSVC )

    TARGET_COMPILE_DEFINITIONS( ${QQMLMODEL_TARGET} PRIVATE -DQQMLMODEL_SHARED )

ELSE()
    ADD_LIBRARY( ${QQMLMODEL_TARGET} STATIC ${QQMLMODEL_SRCS} )
    TARGET_COMPILE_DEFINITIONS( ${QQMLMODEL_TARGET} PUBLIC -DQQMLMODEL_STATIC )
ENDIF()

TARGET_INCLUDE_DIRECTORIES( ${QQMLMODEL_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )
TARGET_COMPILE_DEFINITIONS( ${QQMLMODEL_TARGET} PRIVATE -DQQMLMODEL_VERSION_MAJOR=${QQMLMODEL_VERSION_MAJOR} )
TARGET_COMPILE_DEFINITIONS( ${QQMLMODEL_TARGET} PRIVATE -DQQMLMODEL_VERSION_MINOR=${QQMLMODEL_VERSION_MINOR} )
TARGET_COMPILE_DEFINITIONS( ${QQMLMODEL_TARGET} PRIVATE -DQQMLMODEL_VERSION_PATCH=${QQMLMODEL_VERSION_PATCH} )
TARGET_COMPILE_DEFINITIONS( ${QQMLMODEL_TARGET} PRIVATE -DQQMLMODEL_VERSION_TAG=${QQMLMODEL_VERSION_TAG} )
TARGET_COMPILE_DEFINITIONS( ${QQMLMODEL_TARGET} PRIVATE -DQQMLMODEL_VERSION_TAG_HEX=${QQMLMODEL_VERSION_TAG_HEX} )
IF(QQMLMODEL_USE_NAMESPACE)
TARGET_COMPILE_DEFINITIONS( ${QQMLMODEL_TARGET} PUBLIC -DQQMLMODEL_USE_NAMESPACE )
TARGET_COMPILE_DEFINITIONS( ${QQMLMODEL_TARGET} PUBLIC -DQQMLMODEL_NAMESPACE=${QQMLMODEL_NAMESPACE} )
ENDIF(QQMLMODEL_USE_NAMESPACE)

qt5_use_modules( ${QQMLMODEL_TARGET} Core Qml )

# ┌──────────────────────────────────────────────────────────────────┐
# │                       DOXYGEN                                    │
# └──────────────────────────────────────────────────────────────────┘

IF(QQMLMODEL_BUILD_DOC)
SET( DOXYGEN_BOOTSTRAP_COMPANY OlivierLDff )
SET( DOXYGEN_BOOTSTRAP_PUBLISHER OlivierLDff )
SET( DOXYGEN_BOOTSTRAP_PROJECT_NAME ${QQMLMODEL_TARGET} )
SET( DOXYGEN_BOOTSTRAP_PROJECT_VERSION ${QQMLMODEL_VERSION} )
SET( DOXYGEN_BOOTSTRAP_PROJECT_BRIEF "Additional data models aimed to bring more power to QML applications by using useful C++ models in back-end." )
SET( DOXYGEN_BOOTSTRAP_PROJECT_LOGO )
SET( DOXYGEN_BOOTSTRAP_OUTPUT_DIRECTORY "Docs" )
SET( DOXYGEN_BOOTSTRAP_SRC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src )
SET( DOXYGEN_BOOTSTRAP_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md )
SET( DOXYGEN_TARGET_PREFIX "" )
SET( DOXYGEN_TARGET_SUFFIX "Doc" )
SET( DOXYGEN_BT_REPOSITORY ${QQMLMODEL_DOXYGEN_BT_REPOSITORY} )
SET( DOXYGEN_BT_TAG ${QQMLMODEL_DOXYGEN_BT_TAG} )
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BuildDoxygenBt.cmake)
ENDIF(QQMLMODEL_BUILD_DOC)
