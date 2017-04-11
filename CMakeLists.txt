# Author: Alexander Böhn (with Félix C. Morency)
# © 2011.10 -- GPL, Motherfuckers

# The minimum CMake version required to build this project
cmake_minimum_required(VERSION 3.3)

# Set a new CMake project
project(subjective-c)

option(OBJC_TESTS             "Compile and run tests"                             ON)
option(OBJC_APPS              "Compile and run companion apps"                    ON)
option(OBJC_USE_GCC           "Compile using GCC"                                 OFF)
option(OBJC_RESYMBOLIZE       "Regenerate include/subjective-c/symbols.hpp"       OFF)
option(OBJC_GENERATE_HEADER   "Only generate subjective-c.hpp (do not compile)"   OFF)
option(OBJC_COLOR_TRACE       "Use ANSI color in debug and error trace output"    ON)
option(OBJC_VERBOSE           "Print (highly nerd-oriented) verbose debug output" ON)
option(OBJC_TERMINATOR        "Use a libunwind-based termination handler"         ON)

if(OBJC_USE_GCC)
    # hardcode homebrew path for now
    set(CMAKE_C_COMPILER    "/usr/local/opt/gcc/bin/gcc-5")
    set(CMAKE_CXX_COMPILER  "/usr/local/opt/gcc/bin/g++-5")
endif()

# C++ standard
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
# if(NOT CMAKE_CXX_STANDARD)
#     set(CMAKE_CXX_STANDARD 17)
# endif()

if(OBJC_RESYMBOLIZE)
    execute_process(
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/subjective-c/"
        COMMAND "rm symbols.hpp"
        COMMENT "Deleting generated IOD symbol header file")
endif()

# Go through some stuff
set(subjectivec_VERSION_MAJOR "0")
set(subjectivec_VERSION_MINOR "1")
set(subjectivec_VERSION_PATCH "2")

# If the build script is called from a parent project,
# use the configuration from there.
if(NOT COMMAND if_defined_set)
    set(cmake_directory ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
endif()

# Set the CMake module path to the project cmake directory. This location will
# first be searched for external CMake packages.
set(CMAKE_MODULE_PATH ${cmake_directory})
set(CMAKE_SYSTEM_LIBRARY_PATH ${CMAKE_SYSTEM_LIBRARY_PATH}
    $ENV{LD_LIBRARY_PATH}
    /usr/lib/system)

# Include the library build configuration. This will also include the custom
# macro defined for the project.
include(build_config)

# Include the HalideGenerator.cmake library -- exposing the cmake function
# halide_add_generator_dependency() allowing Halide generator use in-project.
include(HalideProject)
# include(HalideGenerator)

# Include IodSymbolize.cmake from the iod-symbolizer Python tool,
# exposing the cmake function IOD_SYMBOLIZE()
include(IodSymbolize)

# Set the location of the library configuration file if it has not already been
# set. This allow the library to be used by an external project without
# overwriting the variable.
if_defined_set(${PROJECT_NAME}_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
SET(TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
SET(TEST_SCRIPT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests/scripts")
SET(TEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests/include")

# Set up the directories to feed to the IOD symbolizer,
# and the target output header file to generate
SET(IOD_SYMBOLS_HEADER  "${CMAKE_CURRENT_SOURCE_DIR}/include/subjective-c/symbols.hpp")
SET(IOD_SYMBOLIZE_DIR0  "${CMAKE_CURRENT_SOURCE_DIR}/include/subjective-c")
SET(IOD_SYMBOLIZE_DIR1  "${CMAKE_CURRENT_SOURCE_DIR}/python")
SET(IOD_SYMBOLIZE_DIR2  "${CMAKE_CURRENT_SOURCE_DIR}/src")
IOD_SYMBOLIZE(
    ${IOD_SYMBOLS_HEADER}
    ${IOD_SYMBOLIZE_DIR0}
    ${IOD_SYMBOLIZE_DIR1}
    ${IOD_SYMBOLIZE_DIR2})

# Set variables for the deps and apps directories:
SET(DEPS_DIR    "${CMAKE_CURRENT_SOURCE_DIR}/deps")
SET(APPS_DIR    "${CMAKE_CURRENT_SOURCE_DIR}/apps")
SET(HALOGEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/halogen")

# Set variables for the individual deps directories:
SET(IOD_DIR "${DEPS_DIR}/iod")
SET(IOD_TEST_DIR "${IOD_DIR}/tests")
SET(CROSSGUID_DIR "${DEPS_DIR}/crossguid")
SET(DOCOPT_DIR "${DEPS_DIR}/docopt")
SET(INTERPOL_DIR "${DEPS_DIR}/interpol")
SET(IMAGECOMPRESSION_DIR "${DEPS_DIR}/imagecompression")
SET(AFNETWORKING_DIR "${DEPS_DIR}/AFNetworking")
SET(LIBBF_DIR "${DEPS_DIR}/libbf/src")
SET(MABLOCKCLOSURE_DIR "${DEPS_DIR}/MABlockClosure")
SET(SG14_DIR "${DEPS_DIR}/SG14")
SET(SSZIP_DIR "${DEPS_DIR}/SSZipArchive")

# Set variables for directories for the individual apps:
SET(SUBJECTIVE_C_CONFIG_DIR "${APPS_DIR}/subjective-c-config")
SET(IMPASTE_DIR "${APPS_DIR}/impaste")

# Add DEPS_DIR as a subdirectory ...
add_subdirectory(${DEPS_DIR})

# ... but exclude the IOD test directory from being built:
# set_property(DIRECTORY ${IOD_TEST_DIR}
#              PROPERTY EXCLUDE_FROM_ALL TRUE)
# set_property(DIRECTORY ${IOD_TEST_DIR}
#              PROPERTY TEST)

# Add HALOGEN_DIR -- set up Halide generators:
# add_subdirectory(${HALOGEN_DIR})

# Load the project configuration file. CMake will search in the directory setted
# above for a module file named subjectivecConfig.cmake. The configuration
# file will set the different directories and libraries required by the library:
find_package(${PROJECT_NAME} REQUIRED)

# Find the required dependency libraries:
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(TIFF REQUIRED)
find_package(WEBP REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Halide REQUIRED)
find_package(LibUnwind REQUIRED)

# set(HDF5_USE_STATIC_LIBRARIES false)
# set(HDF5_ROOT /usr/local/opt/hdf5/)
# find_package(HDF5 REQUIRED
#     COMPONENTS C CXX HL)

# Include the project-specific `include` directories:
include_directories(${${PROJECT_NAME}_include_dir})
include_directories("${CMAKE_BINARY_DIR}/../include")
include_directories("${CMAKE_BINARY_DIR}")

# Include the `deps` library `include` directories:
# include_directories("${CROSSGUID_DIR}")
include_directories("${DOCOPT_DIR}")
# include_directories("${IMAGECOMPRESSION_DIR}")
# include_directories("${INTERPOL_DIR}")
# include_directories("${AFNETWORKING_DIR}")
# include_directories("${LIBBF_DIR}")
include_directories("${MABLOCKCLOSURE_DIR}")
# include_directories("${IOD_DIR}")
# include_directories("${SG14_DIR}")
# include_directories("${SSZIP_DIR}")

# Ugh
link_directories(/usr/lib)
link_directories(/usr/local/lib)

# Include the required dependency `include` directories:
include_directories(
    ${PNG_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR}
    ${TIFF_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    ${WEBP_INCLUDE_DIR}
    ${HALIDE_INCLUDE_DIR}
    ${LIBUNWIND_INCLUDE_DIR})

# Set the source files and source-file-specific options
# required to build the library:
include(CMakeProjectFiles.cmake)

# Actually build the library: set up an `OBJECT` target ...
add_library(subjective-c OBJECT ${srcs} ${hdrs})
set_target_properties(subjective-c
    PROPERTIES LIBRARY_OUTPUT_NAME "subjective-c")
set_target_properties(subjective-c
    PROPERTIES LINK_FLAGS ${COMMON_LINK_FLAGS})
add_dependencies(subjective-c "project_header")
# add_dependencies(subjective-c "guid")
# add_dependencies(subjective-c "imagecompression")
# add_dependencies(subjective-c "interpol")
# add_dependencies(subjective-c "AFNetworking")
# add_dependencies(subjective-c "libbf")
add_dependencies(subjective-c "MABlockClosure")
# add_dependencies(subjective-c "iod_symbolize")

# ... and build shared and static target libraries,
# based on the `OBJECT` target:
set_property(
    TARGET subjective-c
    PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(subjective-c_shared SHARED $<TARGET_OBJECTS:subjective-c>)
add_library(subjective-c_static STATIC $<TARGET_OBJECTS:subjective-c>)
set_target_properties(subjective-c_shared
    PROPERTIES LIBRARY_OUTPUT_NAME "subjective-c")
set_target_properties(subjective-c_static
    PROPERTIES ARCHIVE_OUTPUT_NAME "subjective-c")

# Set and pass on compile-time config variable values:
get_property(OBJC_COMPILE_OPTIONS GLOBAL PROPERTY COMPILE_FLAGS)
get_target_property(OBJC_INCLUDE_DIRECTORIES subjective-c INCLUDE_DIRECTORIES)
get_target_property(OBJC_LINK_LIBRARIES subjective-c LINK_LIBRARIES)
get_target_property(OBJC_LINK_FLAGS subjective-c LINK_FLAGS)

# Configure and generate the compile-time project header file:
set(PROJECT_HEADER
    "${PROJECT_BINARY_DIR}/subjective-c/subjective-c.hpp")
configure_file(
    "${hdrs_dir}/subjective-c/subjective-c.hpp.in"
    ${PROJECT_HEADER})
add_custom_target("project_header"
    DEPENDS ${PROJECT_HEADER})
set_source_files_properties(
    ${PROJECT_HEADER}
    PROPERTIES GENERATED TRUE)

# Actually build the library II: electric boogaloo
# ... these two target_link_libraries() calls set up the actual
# for-reals building up in all this:
target_link_libraries(subjective-c_shared
    z png imread
    # guid libbf
    MABlockClosure
    # imagecompression
    # interpol
    ${EXTRA_LIBS}
    ${TIFF_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${WEBP_LIBRARIES}
    ${HALIDE_LIBRARIES})

target_link_libraries(subjective-c_static
    z png imread
    # guid libbf
    MABlockClosure
    # imagecompression
    # interpol
    ${EXTRA_LIBS}
    ${TIFF_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${WEBP_LIBRARIES}
    ${HALIDE_LIBRARIES})

# Add the apps subdirectory, if we're building apps:
if(OBJC_APPS)
    add_subdirectory(${APPS_DIR})
endif(OBJC_APPS)

# Build the tests, if we're building tests:
if(OBJC_TESTS)
    
    # Bring in the tests subdirectory --
    # ... this defines add_subjectivec_test(), see below:
    add_subdirectory(${TEST_SOURCE_DIR})
    
    # add_custom_command(
    #     OUTPUT ${APPS_DIR}/TestDataViewer/TestDataViewer/TestData.plist
    #     COMMAND ${TEST_SCRIPT_DIR}/generate-test-filemap.py > ${APPS_DIR}/TestDataViewer/TestDataViewer/TestData.plist)
    # add_custom_target("test_data_plist"
    #     DEPENDS ${APPS_DIR}/TestDataViewer/TestDataViewer/TestData.plist)
    # set_target_properties("test_data_plist"
    #     PROPERTIES GENERATED TRUE)
    # set_target_properties("test_data_plist"
    #     PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${APPS_DIR}/TestDataViewer/TestDataViewer/TestData.plist)
    # add_dependencies("test_data_plist" subjectivec)
    
    # Generate the test data header, if necessary:
    add_custom_command(
        OUTPUT ${TEST_INCLUDE_DIR}/test_data.hpp
        COMMAND ${TEST_SCRIPT_DIR}/generate-test-header.py > ${TEST_INCLUDE_DIR}/test_data.hpp)
    add_custom_target("test_data_header"
        DEPENDS ${TEST_INCLUDE_DIR}/test_data.hpp)
    set_source_files_properties(
        ${TEST_INCLUDE_DIR}/test_data.hpp
        PROPERTIES GENERATED TRUE)
    
    # Set up the `subjective-c_tests` dependencies
    set(subjective-c_tests "test_${PROJECT_NAME}")
    add_executable(subjective-c_tests ${TEST_SOURCES})
    set_target_properties(subjective-c_tests
        PROPERTIES LINK_FLAGS ${COMMON_LINK_FLAGS})
    add_dependencies("test_data_header" subjective-c)
    add_dependencies(subjective-c_tests "project_header")
    add_dependencies(subjective-c_tests "test_data_header")
    add_dependencies(subjective-c_tests "impaste")
    # add_dependencies(subjective-c_tests "iod_symbolize")
    # add_dependencies(subjective-c_tests "guid")
    # add_dependencies(subjective-c_tests "imagecompression")
    # add_dependencies(subjective-c_tests "interpol")
    # add_dependencies(subjective-c_tests "libbf")
    add_dependencies(subjective-c_tests "MABlockClosure")
    # add_dependencies(subjective-c_tests "AFNetworking")
    # add_dependencies(subjective-c_tests "sszip")
    # add_dependencies(subjective-c_tests "subjectivec")
    link_directories(${CMAKE_BINARY_DIR})
    
    # Link the `subjectivec_tests` executable
    target_link_libraries(subjective-c_tests
        subjective-c_shared
        fmt
        # sszip
        # AFNetworking
        MABlockClosure
        # imagecompression
        # interpol
        # libbf guid
        png z
        ${EXTRA_LIBS}
        ${TIFF_LIBRARIES}
        ${JPEG_LIBRARIES}
        ${WEBP_LIBRARIES}
        ${HALIDE_LIBRARIES})
    
    # Set up ctest and cdash:
    enable_testing()
    include(CTest)
    
    # Set up individual test suites --
    # … the add_subjectivec_test() macro is defined in tests/CMakeLists.txt:
    add_subjectivec_test("appkit-copy-paste")
    # add_subjectivec_test("apple-io")
    # add_subjectivec_test("blockhash")
    # add_subjectivec_test("byte-source-gzio")
    # add_subjectivec_test("byte-source-iterators")
    # add_subjectivec_test("filesystem")
    # add_subjectivec_test("gif-write")
    # add_subjectivec_test("halide-io")
    # add_subjectivec_test("hdf5-io")
    # add_subjectivec_test("imageformat-options")
    add_subjectivec_test("impaste-clt")
    # add_subjectivec_test("imageview")
    add_subjectivec_test("json-block-traverse")
    # add_subjectivec_test("libguid")
    add_subjectivec_test("nsdictionary-options-map")
    add_subjectivec_test("nsurl-image-types")
    add_subjectivec_test("objc-rt")
    # add_subjectivec_test("refcount")
    add_subjectivec_test("sfinae")
    # add_subjectivec_test("libsszip")
    # add_subjectivec_test("terminator")
    # add_subjectivec_test("interleaved-io")
    
endif(OBJC_TESTS)

# Install the built libraries and header files, as appropriate:
install(TARGETS subjective-c_shared DESTINATION lib)
install(TARGETS subjective-c_static DESTINATION lib)
install(DIRECTORY ${PROJECT_BINARY_DIR}/subjective-c/ DESTINATION include/subjective-c
  FILES_MATCHING PATTERN "subjective-c.hpp")
install(DIRECTORY include/subjective-c/private/ DESTINATION include/subjective-c/private
  FILES_MATCHING PATTERN "*.h")
install(DIRECTORY include/subjective-c/ DESTINATION include/subjective-c
  FILES_MATCHING PATTERN "*.h")
install(DIRECTORY include/subjective-c/ DESTINATION include/subjective-c
  FILES_MATCHING PATTERN "*.hh")
install(DIRECTORY include/subjective-c/ DESTINATION include/subjective-c
  FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY cmake/ DESTINATION share/subjective-c
  FILES_MATCHING PATTERN "*subjective-c-config.cmake")
