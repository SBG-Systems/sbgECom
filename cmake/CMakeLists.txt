# CMake rules used to build the public sbgECom library release
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(sbgECom)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Use a sbgCommonLin as a static library
add_definitions(-DSBG_COMMON_STATIC_USE -D_CRT_SECURE_NO_WARNINGS)

include_directories(
	${PROJECT_SOURCE_DIR}/../src/
	${PROJECT_SOURCE_DIR}/../common/
)

# Define easier to find output paths
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin/)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin/)

file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/../src/*.c)
file(GLOB_RECURSE COMMON_SRC ${PROJECT_SOURCE_DIR}/../common/*.c)

# Exclude platform specific files
if (NOT MSVC)
	list(REMOVE_ITEM COMMON_SRC "${PROJECT_SOURCE_DIR}/../common/interfaces/sbgInterfaceSerialWin.c")
else ()
	list(REMOVE_ITEM COMMON_SRC "${PROJECT_SOURCE_DIR}/../common/interfaces/sbgInterfaceSerialUnix.c")
endif()

add_library(sbgECom STATIC ${SRC} ${COMMON_SRC})

if (MSVC)
	target_link_libraries(sbgECom Ws2_32)
	#target_compile_definitions(sbgECom PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Add all examples
add_executable(airDataInput "${PROJECT_SOURCE_DIR}/../examples/airDataInput/src/airDataInput.c")
target_link_libraries(airDataInput sbgECom)
add_dependencies(airDataInput sbgECom)

add_executable(ellipseMinimal "${PROJECT_SOURCE_DIR}/../examples/ellipseMinimal/src/ellipseMinimal.c")
target_link_libraries(ellipseMinimal sbgECom)
add_dependencies(ellipseMinimal sbgECom)

add_executable(ellipseOnboardMagCalib "${PROJECT_SOURCE_DIR}/../examples/ellipseOnboardMagCalib/src/ellipseOnboardMagCalib.c")
target_link_libraries(ellipseOnboardMagCalib sbgECom)
add_dependencies(ellipseOnboardMagCalib sbgECom)

add_executable(hpInsMinimal "${PROJECT_SOURCE_DIR}/../examples/hpInsMinimal/src/hpInsMinimal.c")
target_link_libraries(hpInsMinimal sbgECom)
add_dependencies(hpInsMinimal sbgECom)

add_executable(pulseMinimal "${PROJECT_SOURCE_DIR}/../examples/pulseMinimal/src/pulseMinimal.c")
target_link_libraries(pulseMinimal sbgECom)
add_dependencies(pulseMinimal sbgECom)
