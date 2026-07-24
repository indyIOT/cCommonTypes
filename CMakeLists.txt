################################################################################
 # @file CMakeLists.txt
 # @brief CMake configuration for the cCommonTypes library.
 #
 # This file sets up the build configuration for the cCommonTypes library using CMake.
 # It specifies the minimum required CMake version, project name, version, and language.
 # It also defines the C++ standard to be used and creates an interface library target.
 #
 # Builds the cCommonTypes library as a header-only Interface target
 # Usage:
    #   1. Create a build directory: mkdir build
    #   2. Navigate to the build directory: cd build
    #   3. Generate build files: cmake ..
    #   4. Build the project: cmake --build .
    #   5. Run the executable: ./Executable
    #   6. To only build the library: cmake --build . --target cCommonTypes
########################################################################################

cmake_minimum_required(VERSION 3.31)

project(cCommonTypes VERSION 1.0.0 LANGUAGES C CXX)

set ( CMAKE_CXX_STANDARD 14)
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_EXTENSIONS ON )

project(cCommonTypes VERSION 1.0.0 LANGUAGES C)

set(COMMON_TYPES_LIBRARY_NAME commonTypes)
set(COMMON_TYPES_BUILD_TYPE_STATIC ON)
set(VERSION_HEADER_DIR  ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(VERSION_HEADER_FILE ${VERSION_HEADER_DIR}/cCommonTypesVersion.h)
if( COMMON_TYPES_BUILD_TYPE_STATIC )
    set(COMMON_TYPES_BUILD_TYPE "STATIC")
    set(COMMON_TYPES_BUILD_TYPE_ENUM STATIC_LIBRARY_BUILD)
else()
    set(COMMON_TYPES_BUILD_TYPE "DYNAMIC")
    set(COMMON_TYPES_BUILD_TYPE_ENUM DYNAMIC_LIBRARY_BUILD)
endif()

add_library(${COMMON_TYPES_LIBRARY_NAME} INTERFACE)
add_library(cCommonTypes ALIAS ${COMMON_TYPES_LIBRARY_NAME})

# Write a stub at configure time so the path exists and dependency scanning works
# before the real file is generated on the first build.
file(MAKE_DIRECTORY ${VERSION_HEADER_DIR})
if(NOT EXISTS ${VERSION_HEADER_FILE})
    file(WRITE ${VERSION_HEADER_FILE}
        "/* Stub — replaced at build time by generateVersion.cmake */\n"
        "#ifndef C_COMMON_TYPES_VERSION_H\n"
        "#define C_COMMON_TYPES_VERSION_H\n"
        "#endif /* C_COMMON_TYPES_VERSION_H */\n"
    )
endif()

target_include_directories(${COMMON_TYPES_LIBRARY_NAME}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/publicInclude
)

# Expose the generated header directory so consumers can #include it
target_include_directories(${COMMON_TYPES_LIBRARY_NAME}
    INTERFACE
        ${VERSION_HEADER_DIR}
)

# Always regenerate cCommonTypesVersion.h at every build invocation.
# add_custom_target runs unconditionally (no output tracking), ensuring a
# fresh timestamp in PATCH on every cmake --build call.
add_custom_target(GenerateCommonTypesVersion ALL
    COMMAND ${CMAKE_COMMAND}
        -DVERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        -DVERSION_MINOR=${PROJECT_VERSION_MINOR}
        -DVERSION_PATCH=${PROJECT_VERSION_PATCH}
        -DVERSION_BUILD_TYPE=${COMMON_TYPES_BUILD_TYPE}
        -DVERSION_BUILD_TYPE_ENUM=${COMMON_TYPES_BUILD_TYPE_ENUM}
        -DOUTPUT_FILE=${VERSION_HEADER_FILE}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/generateVersion.cmake
    COMMENT "Regenerating cCommonTypesVersion.h"
    VERBATIM
)

add_dependencies(${COMMON_TYPES_LIBRARY_NAME} GenerateCommonTypesVersion)
