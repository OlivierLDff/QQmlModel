#
#	QQmlModel CMake script by Olivier Le Doeuff

#
## CMAKE INPUT
#
#	- QQML_MODEL_TARGET : Name of the library target. Default : "QQmlModel"
#	- QQML_MODEL_PROJECT : Name of the project. Default : "QQmlModel"
#	- QQML_MODEL_BUILD_SHARED : Build shared library [ON OFF]. Default: OFF.
#	- QQML_MODEL_BUILD_STATIC : Build static library [ON OFF]. Default: ON.
#	- QQML_MODEL_USE_NAMESPACE : If the library compile with a namespace. Default: OFF.
#	- QQML_MODEL_NAMESPACE : Namespace for the library. Only relevant if QQML_MODEL_USE_NAMESPACE is ON. Default: "Qqm".
#	- QQML_MODEL_BUILD_DOC : Build the QQmlModel Doc [ON OFF]. Default: OFF.
#	- QQML_MODEL_DOXYGEN_BT_REPOSITORY : Repository of DoxygenBt. Default : "https://github.com/OlivierLDff/DoxygenBootstrapped.git"
#	- QQML_MODEL_DOXYGEN_BT_TAG : Git Tag of DoxygenBt. Default : "v1.3.0"

#
## CMAKE OUTPUT
#
#	- QQML_MODEL_TARGET : Output target to link to. Default: QQmlModel
#

#
## VERSION
#	- 1.0.0 : 05/12/2018 : Initial Work
# 	- 1.0.1 : Fix DoxygenBootstrapped github link
# 	- 1.0.4 : Fix CMakeLists filename

# ┌──────────────────────────────────────────────────────────────────┐
# │						  CMAKE PROPERTIES							 │
# └──────────────────────────────────────────────────────────────────┘

CMAKE_MINIMUM_REQUIRED( VERSION 3.1.0 )

# ┌──────────────────────────────────────────────────────────────────┐
# │						  PROJECT SETTINGS							 │
# └──────────────────────────────────────────────────────────────────┘

SET( QQML_MODEL_TARGET "QQmlModel" CACHE STRING "Target Name" )
SET( QQML_MODEL_PROJECT "QQmlModel" CACHE STRING "Project Name")
SET( QQML_MODEL_BUILD_SHARED OFF CACHE BOOL "Build shared lib for QQmlModel" )
IF(NOT QQML_MODEL_BUILD_SHARED)
SET( QQML_MODEL_BUILD_STATIC ON CACHE BOOL "Build static lib for QQmlModel" )
ELSE(NOT QQML_MODEL_BUILD_SHARED)
SET( QQML_MODEL_BUILD_STATIC OFF CACHE BOOL "Build static lib for QQmlModel" )
ENDIF(NOT QQML_MODEL_BUILD_SHARED)
SET( QQML_MODEL_USE_NAMESPACE OFF CACHE BOOL "If the library compile with a namespace.")
SET( QQML_MODEL_NAMESPACE "Qqm" CACHE STRING "Namespace for the library. Only relevant if QQML_MODEL_USE_NAMESPACE is ON")
SET( QQML_MODEL_BUILD_DOC OFF CACHE BOOL "Build QQmlModel Doc with Doxygen" )
IF(QQML_MODEL_BUILD_DOC)
SET( QQML_MODEL_DOXYGEN_BT_REPOSITORY "https://github.com/OlivierLDff/DoxygenBootstrappedCMake.git" CACHE STRING "Repository of DoxygenBt" )
SET( QQML_MODEL_DOXYGEN_BT_TAG v1.3.2 CACHE STRING "Git Tag of DoxygenBt" )
ENDIF(QQML_MODEL_BUILD_DOC)

PROJECT( ${QQML_MODEL_PROJECT} )
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# ┌──────────────────────────────────────────────────────────────────┐
# │						  	QT5 CMAKE 								 │
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

FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Qml REQUIRED)

# ┌──────────────────────────────────────────────────────────────────┐
# │						  VERSION 	 								 │
# └──────────────────────────────────────────────────────────────────┘

# Project version must be set here
SET(QQML_MODEL_VERSION_MAJOR 1)
SET(QQML_MODEL_VERSION_MINOR 0)
SET(QQML_MODEL_VERSION_PATCH 4)
SET(QQML_MODEL_VERSION ${QQML_MODEL_VERSION_MAJOR}.${QQML_MODEL_VERSION_MINOR}.${QQML_MODEL_VERSION_PATCH} CACHE STRING "Version")

IF( EXISTS "${CMAKE_SOURCE_DIR}/.git" )
	EXECUTE_PROCESS(
		COMMAND git describe --always
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE QQML_MODEL_VERSION_TAG
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
ELSE()
	SET( QQML_MODEL_VERSION_TAG 00000000 )
ENDIF()
SET( QQML_MODEL_VERSION_TAG_HEX 0x${QQML_MODEL_VERSION_TAG})

# ┌──────────────────────────────────────────────────────────────────┐
# │						  STATUS 	 								 │
# └──────────────────────────────────────────────────────────────────┘

MESSAGE( STATUS  "------ ${QQML_MODEL_TARGET} Configuration ${QQML_MODEL_VERSION} ------" )

MESSAGE( STATUS "QQML_MODEL_TARGET                 : ${QQML_MODEL_TARGET}" )
MESSAGE( STATUS "QQML_MODEL_PROJECT                : ${QQML_MODEL_PROJECT}" )
MESSAGE( STATUS "QQML_MODEL_VERSION                : ${QQML_MODEL_VERSION}" )
MESSAGE( STATUS "QQML_MODEL_VERSION_TAG_HEX        : ${QQML_MODEL_VERSION_TAG_HEX}" )
MESSAGE( STATUS "QQML_MODEL_BUILD_SHARED           : ${QQML_MODEL_BUILD_SHARED}" )
MESSAGE( STATUS "QQML_MODEL_BUILD_STATIC           : ${QQML_MODEL_BUILD_STATIC}" )
MESSAGE( STATUS "QQML_MODEL_USE_NAMESPACE          : ${QQML_MODEL_USE_NAMESPACE}" )
IF(QQML_MODEL_USE_NAMESPACE)
MESSAGE( STATUS "QQML_MODEL_NAMESPACE              : ${QQML_MODEL_NAMESPACE}" )
ENDIF(QQML_MODEL_USE_NAMESPACE)
MESSAGE( STATUS "QQML_MODEL_BUILD_DOC              : ${QQML_MODEL_BUILD_DOC}" )
IF(QQML_MODEL_BUILD_DOC)
MESSAGE( STATUS "QQML_MODEL_DOXYGEN_BT_REPOSITORY  : ${QQML_MODEL_DOXYGEN_BT_REPOSITORY}" )
MESSAGE( STATUS "QQML_MODEL_DOXYGEN_BT_TAG         : ${QQML_MODEL_DOXYGEN_BT_TAG}" )
ENDIF(QQML_MODEL_BUILD_DOC)

MESSAGE( STATUS "------ ${QQML_MODEL_TARGET} End Configuration ------" )

IF((QQML_MODEL_BUILD_SHARED AND QQML_MODEL_BUILD_STATIC) OR (NOT QQML_MODEL_BUILD_SHARED AND NOT QQML_MODEL_BUILD_STATIC))
MESSAGE( FATAL_ERROR "Set either Build shared or build static to ON, can't build both at once" )
ENDIF((QQML_MODEL_BUILD_SHARED AND QQML_MODEL_BUILD_STATIC) OR (NOT QQML_MODEL_BUILD_SHARED AND NOT QQML_MODEL_BUILD_STATIC))

# ┌──────────────────────────────────────────────────────────────────┐
# │						  SOURCES 	 								 │
# └──────────────────────────────────────────────────────────────────┘

SET( QQML_MODEL_SRCS 
	# Main
	${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlObjectListModel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlObjectListModel.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlVariantListModel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlVariantListModel.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlModelShared.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlModelShared.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlObjectListModel
	${CMAKE_CURRENT_SOURCE_DIR}/src/QQmlVariantListModel
	)

# ┌──────────────────────────────────────────────────────────────────┐
# │						  TARGET 	 								 │
# └──────────────────────────────────────────────────────────────────┘

IF(QQML_MODEL_BUILD_SHARED)
	IF( MSVC )
	INCLUDE (GenerateExportHeader)
	ENDIF( MSVC )

	ADD_LIBRARY( ${QQML_MODEL_TARGET} SHARED ${QQML_MODEL_SRCS} )

	IF( MSVC )
	GENERATE_EXPORT_HEADER( ${QQML_MODEL_TARGET}
		BASE_NAME ${QQML_MODEL_TARGET}
		EXPORT_MACRO_NAME ${QQML_MODEL_TARGET}_EXPORT
		EXPORT_FILE_NAME ${QQML_MODEL_TARGET}_Export.h
		STATIC_DEFINE ${QQML_MODEL_TARGET}_BUILT_AS_STATIC
	)
	ENDIF( MSVC )
	
	TARGET_COMPILE_DEFINITIONS( ${QQML_MODEL_TARGET} PRIVATE -DQQML_MODEL_SHARED )

ELSE()
	ADD_LIBRARY( ${QQML_MODEL_TARGET} STATIC ${QQML_MODEL_SRCS} )
	TARGET_COMPILE_DEFINITIONS( ${QQML_MODEL_TARGET} PUBLIC -DQQML_MODEL_STATIC )
ENDIF()

TARGET_INCLUDE_DIRECTORIES( ${QQML_MODEL_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )
TARGET_COMPILE_DEFINITIONS( ${QQML_MODEL_TARGET} PRIVATE -DQQML_MODEL_VERSION_MAJOR=${QQML_MODEL_VERSION_MAJOR} )
TARGET_COMPILE_DEFINITIONS( ${QQML_MODEL_TARGET} PRIVATE -DQQML_MODEL_VERSION_MINOR=${QQML_MODEL_VERSION_MINOR} )
TARGET_COMPILE_DEFINITIONS( ${QQML_MODEL_TARGET} PRIVATE -DQQML_MODEL_VERSION_PATCH=${QQML_MODEL_VERSION_PATCH} )
TARGET_COMPILE_DEFINITIONS( ${QQML_MODEL_TARGET} PRIVATE -DQQML_MODEL_VERSION_TAG=${QQML_MODEL_VERSION_TAG} )
TARGET_COMPILE_DEFINITIONS( ${QQML_MODEL_TARGET} PRIVATE -DQQML_MODEL_VERSION_TAG_HEX=${QQML_MODEL_VERSION_TAG_HEX} )
IF(QQML_MODEL_USE_NAMESPACE)
TARGET_COMPILE_DEFINITIONS( ${QQML_MODEL_TARGET} PUBLIC -DQQML_MODEL_USE_NAMESPACE )
TARGET_COMPILE_DEFINITIONS( ${QQML_MODEL_TARGET} PUBLIC -DQQML_MODEL_NAMESPACE=${QQML_MODEL_NAMESPACE} )
ENDIF(QQML_MODEL_USE_NAMESPACE)

qt5_use_modules( ${QQML_MODEL_TARGET} Core Qml )

# ┌──────────────────────────────────────────────────────────────────┐
# │						  DOXYGEN 									 │
# └──────────────────────────────────────────────────────────────────┘

IF(QQML_MODEL_BUILD_DOC)
SET( DOXYGEN_BOOTSTRAP_COMPANY OlivierLDff )
SET( DOXYGEN_BOOTSTRAP_PUBLISHER OlivierLDff )
SET( DOXYGEN_BOOTSTRAP_PROJECT_NAME ${QQML_MODEL_TARGET} )
SET( DOXYGEN_BOOTSTRAP_PROJECT_VERSION ${QQML_MODEL_VERSION} )
SET( DOXYGEN_BOOTSTRAP_PROJECT_BRIEF "Additional data models aimed to bring more power to QML applications by using useful C++ models in back-end." )
SET( DOXYGEN_BOOTSTRAP_PROJECT_LOGO )
SET( DOXYGEN_BOOTSTRAP_OUTPUT_DIRECTORY "Docs" )
SET( DOXYGEN_BOOTSTRAP_SRC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src )
SET( DOXYGEN_BOOTSTRAP_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md )
SET( DOXYGEN_TARGET_PREFIX "" )
SET( DOXYGEN_TARGET_SUFFIX "Doc" )
SET( DOXYGEN_BT_REPOSITORY ${QQML_MODEL_DOXYGEN_BT_REPOSITORY} )
SET( DOXYGEN_BT_TAG ${QQML_MODEL_DOXYGEN_BT_TAG} )
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BuildDoxygenBt.cmake)
ENDIF(QQML_MODEL_BUILD_DOC)