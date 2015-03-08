# Author: Félix C. Morency
# 2011.10

# To keep the file list clean
set(hdrs_dir ${${PROJECT_NAME}_include_dir})
set(srcs_dir ${CMAKE_CURRENT_SOURCE_DIR}/${source_dir})

# Configure the project-settings header file
configure_file(
    "${hdrs_dir}/libimread.hpp.in"
    "${PROJECT_BINARY_DIR}/libimread/libimread.hpp")

# Project header files
set(hdrs
    ${PROJECT_BINARY_DIR}/libimread/libimread.hpp
    ${hdrs_dir}/fmemopen/fmemopen.hh
    ${hdrs_dir}/fmemopen/open_memstream.hh
    ${hdrs_dir}/private/buffer_t.h
    ${hdrs_dir}/private/image_io.h
    ${hdrs_dir}/private/spx_defines.h
    ${hdrs_dir}/private/static_image.h
    ${hdrs_dir}/UTI/UTI.h
    ${hdrs_dir}/_apple.hh
    ${hdrs_dir}/_bmp.hh
    ${hdrs_dir}/_jpeg.hh
    ${hdrs_dir}/_lsm.hh
    ${hdrs_dir}/_png.hh
    ${hdrs_dir}/_pvrtc.hh
    ${hdrs_dir}/_tiff.hh
    ${hdrs_dir}/_webp.hh
    ${hdrs_dir}/base.hh
    ${hdrs_dir}/errors.hh
    ${hdrs_dir}/file.hh
    ${hdrs_dir}/formats.hh
    ${hdrs_dir}/halide.hh
    ${hdrs_dir}/memory.hh
    # ${hdrs_dir}/numpy.hh
    ${hdrs_dir}/options.hh
    ${hdrs_dir}/pvr.hh
    ${hdrs_dir}/tools.hh
)

# Project source files
set(srcs
    ${srcs_dir}/fmemopen/fmemopen.cpp
    ${srcs_dir}/fmemopen/open_memstream.cpp
    ${srcs_dir}/UTI/UTI.mm
    ${srcs_dir}/_apple.mm
    ${srcs_dir}/_bmp.cpp
    ${srcs_dir}/_jpeg.cpp
    ${srcs_dir}/_lsm.cpp
    ${srcs_dir}/_png.cpp
    ${srcs_dir}/_pvrtc.cpp
    ${srcs_dir}/_tiff.cpp
    ${srcs_dir}/_webp.cpp
    ${srcs_dir}/file.cpp
    ${srcs_dir}/formats.cpp
    ${srcs_dir}/lzw.cpp
    # ${srcs_dir}/numpy.cpp
    ${srcs_dir}/halide.cpp
    ${srcs_dir}/pvr.cpp
    ${srcs_dir}/pvrtc.cpp
)
