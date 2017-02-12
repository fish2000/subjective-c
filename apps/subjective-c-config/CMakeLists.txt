# Author: Alexander Böhn (with Félix C. Morency)
# © 2011.10 -- GPL, Motherfuckers
cmake_minimum_required(VERSION 3.1)
project(subjectivec-config)

set(subjectivec-config_VERSION_MAJOR "0")
set(subjectivec-config_VERSION_MINOR "1")
set(subjectivec-config_VERSION_PATCH "0")

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
    subjectivec_shared
    docopt)

set(hdrs "subjectivec-config.h")
set(srcs "main.cpp")
add_executable("subjectivec-config" ${srcs} ${hdrs})
target_link_libraries("subjectivec-config" ${EXTRA_LIBS})
add_dependencies("subjectivec-config" imread)
add_dependencies("subjectivec-config" docopt)
install(TARGETS "subjectivec-config" DESTINATION bin)
