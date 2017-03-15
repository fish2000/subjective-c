# Author: Alexander Böhn (with Félix C. Morency)
# © 2011.10 -- GPL, Motherfuckers
cmake_minimum_required(VERSION 3.1)
project(subjective-c-config)

set(subjective-c-config_VERSION_MAJOR "0")
set(subjective-c-config_VERSION_MINOR "1")
set(subjective-c-config_VERSION_PATCH "0")

find_path(LIBDOCOPT_INCLUDE_DIR NAMES docopt.h
                                PATHS ${DOCOPT_DIR})

include_directories(
    ${subjectivec_include_dir}
    ${LIBDOCOPT_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(
    -Wall -Werror -O3
    -std=c++14 -stdlib=libc++
    -fstrict-aliasing
    -fstack-protector-all
    -mtune=native)

set(EXTRA_LIBS
    ${EXTRA_LIBS}
    subjective-c_shared
    docopt)

set(hdrs "subjective-c-config.h")
set(srcs "main.cpp")
add_executable("subjective-c-config" ${srcs} ${hdrs})
target_link_libraries("subjective-c-config" ${EXTRA_LIBS})
add_dependencies("subjective-c-config" subjective-c)
add_dependencies("subjective-c-config" docopt)
install(TARGETS "subjective-c-config" DESTINATION bin)
