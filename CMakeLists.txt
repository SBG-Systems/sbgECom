cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Release CACHE STRING "build type")

project(sbgECom)

#
# Project configuration
#
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TOOLS "Build tools" OFF)

#
# sbgECom
#
add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE COMMON_SRC ${PROJECT_SOURCE_DIR}/common/*.c)
file(GLOB_RECURSE ECOM_SRC ${PROJECT_SOURCE_DIR}/src/*.c)

# Exclude platform specific files
if (NOT MSVC)
	list(REMOVE_ITEM COMMON_SRC ${PROJECT_SOURCE_DIR}/common/interfaces/sbgInterfaceSerialWin.c)
else ()
	list(REMOVE_ITEM COMMON_SRC ${PROJECT_SOURCE_DIR}/common/interfaces/sbgInterfaceSerialUnix.c)
endif()

target_sources(${PROJECT_NAME} PRIVATE ${COMMON_SRC} ${ECOM_SRC})

target_include_directories(${PROJECT_NAME}
	PRIVATE
		${PROJECT_SOURCE_DIR}/src
		${PROJECT_SOURCE_DIR}/common
	INTERFACE
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>)

target_compile_definitions(${PROJECT_NAME} PUBLIC SBG_COMMON_STATIC_USE)

if (MSVC)
	target_compile_definitions(${PROJECT_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)
	target_link_libraries(${PROJECT_NAME} PUBLIC Ws2_32)
endif()

#
# Examples
#
if (BUILD_EXAMPLES)
	add_executable(airDataInput ${PROJECT_SOURCE_DIR}/examples/airDataInput/src/airDataInput.c)
	target_link_libraries(airDataInput ${PROJECT_NAME})
	install(TARGETS airDataInput DESTINATION bin/examples COMPONENT executables)

	add_executable(ellipseMinimal ${PROJECT_SOURCE_DIR}/examples/ellipseMinimal/src/ellipseMinimal.c)
	target_link_libraries(ellipseMinimal ${PROJECT_NAME})
	install(TARGETS ellipseMinimal DESTINATION bin/examples COMPONENT executables)

	add_executable(ellipseOnboardMagCalib ${PROJECT_SOURCE_DIR}/examples/ellipseOnboardMagCalib/src/ellipseOnboardMagCalib.c)
	target_link_libraries(ellipseOnboardMagCalib ${PROJECT_NAME})
	install(TARGETS ellipseOnboardMagCalib DESTINATION bin/examples COMPONENT executables)

	add_executable(hpInsMinimal ${PROJECT_SOURCE_DIR}/examples/hpInsMinimal/src/hpInsMinimal.c)
	target_link_libraries(hpInsMinimal ${PROJECT_NAME})
	install(TARGETS hpInsMinimal DESTINATION bin/examples COMPONENT executables)

	add_executable(pulseMinimal ${PROJECT_SOURCE_DIR}/examples/pulseMinimal/src/pulseMinimal.c)
	target_link_libraries(pulseMinimal ${PROJECT_NAME})
	install(TARGETS pulseMinimal DESTINATION bin/examples COMPONENT executables)
endif(BUILD_EXAMPLES)

#
# Tools
#
if (BUILD_TOOLS)
	include(FetchContent)

	FetchContent_Declare(argtable3
		GIT_REPOSITORY https://github.com/argtable/argtable3.git
		GIT_TAG v3.1.5.1c1bb23
	)

	FetchContent_GetProperties(argtable3)

	if (NOT argtable3_POPULATED)
		FetchContent_Populate(argtable3)
		add_subdirectory(${argtable3_SOURCE_DIR} ${argtable3_BINARY_DIR} EXCLUDE_FROM_ALL)
	endif()

	add_executable(sbgBasicLogger
		${PROJECT_SOURCE_DIR}/tools/sbgBasicLogger/src/main.c
		${PROJECT_SOURCE_DIR}/tools/sbgBasicLogger/src/sbgBasicLoggerAccumulators.h
		${PROJECT_SOURCE_DIR}/tools/sbgBasicLogger/src/sbgBasicLoggerFile.h
		${PROJECT_SOURCE_DIR}/tools/sbgBasicLogger/src/sbgBasicLoggerHandler.h
		${PROJECT_SOURCE_DIR}/tools/sbgBasicLogger/src/sbgBasicLoggerAccumulators.c
		${PROJECT_SOURCE_DIR}/tools/sbgBasicLogger/src/sbgBasicLoggerFile.c
		${PROJECT_SOURCE_DIR}/tools/sbgBasicLogger/src/sbgBasicLoggerHandler.c
	)
	target_include_directories(sbgBasicLogger PRIVATE ${argtable3_SOURCE_DIR}/src)
	target_link_libraries(sbgBasicLogger ${PROJECT_NAME} argtable3_static)
	install(TARGETS sbgBasicLogger DESTINATION bin/tools/sbgBasicLogger COMPONENT executables)
	install(FILES tools/sbgBasicLogger/README.md DESTINATION bin/tools/sbgBasicLogger COMPONENT executables)

	add_executable(sbgEComApi ${PROJECT_SOURCE_DIR}/tools/sbgEComApi/src/main.c)
	target_include_directories(sbgEComApi PRIVATE ${argtable3_SOURCE_DIR}/src)
	target_link_libraries(sbgEComApi ${PROJECT_NAME} argtable3_static)
	install(TARGETS sbgEComApi DESTINATION bin/tools/sbgEComApi COMPONENT executables)
	install(FILES tools/sbgEComApi/README.md DESTINATION bin/tools/sbgEComApi COMPONENT executables)
endif(BUILD_TOOLS)

#
# Install
#
install(TARGETS ${PROJECT_NAME} EXPORT sbgEComTargets)
install(EXPORT sbgEComTargets
        FILE sbgEComTargets.cmake
        NAMESPACE sbg::
        DESTINATION lib/cmake/sbg)
install(DIRECTORY common/ TYPE INCLUDE FILES_MATCHING REGEX ".*\\.h")
install(DIRECTORY src/ TYPE INCLUDE FILES_MATCHING REGEX ".*\\.h")
