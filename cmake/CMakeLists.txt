# CMake rules used to build the public sbgECom library release
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Determine whether this is a standalone project or included by other project
set( SBGECOM_SDK_STANDALONE_PROJECT OFF )
if( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
  set( SBGECOM_SDK_STANDALONE_PROJECT ON ) 
endif()

project(sbgECom)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
add_library(sbgECom::libsbgECom ALIAS sbgECom)

# Include information for the target
target_include_directories( sbgECom
  PUBLIC
    ${PROJECT_SOURCE_DIR}/../src/
		${PROJECT_SOURCE_DIR}/../common/
)

# Use a sbgCommonLib as a static library
target_compile_definitions( sbgECom 
	PRIVATE
	  -DSBG_COMMON_STATIC_USE 
		-D_CRT_SECURE_NO_WARNINGS
)

if (MSVC)
	target_link_libraries(sbgECom Ws2_32)
	#target_compile_definitions(sbgECom PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# skip examples when part of an external project
if( SBGECOM_SDK_STANDALONE_PROJECT )
	# Add all examples
	add_executable(airDataInput "${PROJECT_SOURCE_DIR}/../examples/airDataInput/src/airDataInput.c")
	target_link_libraries(airDataInput PRIVATE sbgECom::libsbgECom )

	add_executable(ellipseMinimal "${PROJECT_SOURCE_DIR}/../examples/ellipseMinimal/src/ellipseMinimal.c")
	target_link_libraries(ellipseMinimal PRIVATE sbgECom::libsbgECom )

	add_executable(ellipseOnboardMagCalib "${PROJECT_SOURCE_DIR}/../examples/ellipseOnboardMagCalib/src/ellipseOnboardMagCalib.c")
	target_link_libraries(ellipseOnboardMagCalib PRIVATE sbgECom::libsbgECom )

	add_executable(hpInsMinimal "${PROJECT_SOURCE_DIR}/../examples/hpInsMinimal/src/hpInsMinimal.c")
	target_link_libraries(hpInsMinimal PRIVATE sbgECom::libsbgECom )

	add_executable(pulseMinimal "${PROJECT_SOURCE_DIR}/../examples/pulseMinimal/src/pulseMinimal.c")
	target_link_libraries(pulseMinimal PRIVATE sbgECom::libsbgECom )
endif()