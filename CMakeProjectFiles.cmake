# Author: Alexander Böhn (with Félix C. Morency)
# © 2011.10 -- GPL, Motherfuckers

# To keep the file list clean
set(hdrs_dir ${${PROJECT_NAME}_include_dir})
set(srcs_dir ${CMAKE_CURRENT_SOURCE_DIR})

SET(COMMON_LINK_FLAGS
    -m64 -mmacosx-version-min=10.9
    -fobjc-link-runtime)

# language-specific compile options
SET(C_OPTIONS
    -std=c99
    -Wno-incompatible-pointer-types
    -Wno-char-subscripts
    -x c)

SET(CXX_OPTIONS
    -std=c++1z -stdlib=libc++
    -x c++)

SET(OBJC_OPTIONS
    -fstack-protector
    -fobjc-abi-version=3
    -fno-objc-arc
    -fobjc-legacy-dispatch
    -std=c99 -ObjC
    -x objective-c)

SET(OBJCXX_OPTIONS
    -fstack-protector
    -fobjc-abi-version=3
    -fno-objc-arc
    -fobjc-legacy-dispatch
    -fobjc-call-cxx-cdtors
    -std=c++1z -stdlib=libc++
    -x objective-c++)

SET(OBJCXX_OPTIONS_ARC
    -fobjc-abi-version=3
    -fobjc-arc -fobjc-call-cxx-cdtors
    -fno-objc-arc-exceptions
    -std=c++1z -stdlib=libc++
    -x objective-c++)

# Extra options for Image IO code
SET(IO_EXTRA_OPTIONS
    -ffast-math)

SET(C_FILES "")
SET(CC_FILES "")
SET(M_FILES "")
SET(MM_FILES "")
SET(IO_FILES "")

FILE(
    GLOB_RECURSE C_FILES
    "${srcs_dir}/*.c")
FILE(
    GLOB_RECURSE CC_FILES
    "${srcs_dir}/*.cpp")
FILE(
    GLOB_RECURSE M_FILES
    "${srcs_dir}/*.m")
FILE(
    GLOB_RECURSE MM_FILES
    "${srcs_dir}/*.mm")
FILE(
    GLOB_RECURSE IO_FILES
    "${srcs_dir}/IO/*.*")

SEPARATE_ARGUMENTS(COMMON_LINK_FLAGS)

SEPARATE_ARGUMENTS(C_FILES)
SEPARATE_ARGUMENTS(CC_FILES)
SEPARATE_ARGUMENTS(M_FILES)
SEPARATE_ARGUMENTS(MM_FILES)
SEPARATE_ARGUMENTS(IO_FILES)

SEPARATE_ARGUMENTS(C_OPTIONS)
SEPARATE_ARGUMENTS(CXX_OPTIONS)
SEPARATE_ARGUMENTS(OBJC_OPTIONS)
SEPARATE_ARGUMENTS(OBJCXX_OPTIONS)
SEPARATE_ARGUMENTS(IO_EXTRA_OPTIONS)

# Project header files
set(hdrs

    ${hdrs_dir}/subjective-c/categories/NSBitmapImageRep+IM.hh
    ${hdrs_dir}/subjective-c/categories/NSColor+IM.hh
    ${hdrs_dir}/subjective-c/categories/NSData+IM.hh
    ${hdrs_dir}/subjective-c/categories/NSDictionary+IM.hh
    # ${hdrs_dir}/subjective-c/categories/NSImage+CGImage.h
    ${hdrs_dir}/subjective-c/categories/NSImage+QuickLook.h
    # ${hdrs_dir}/subjective-c/categories/NSImage+Resize.h
    # ${hdrs_dir}/subjective-c/categories/NSImage+ResizeBestFit.h
    ${hdrs_dir}/subjective-c/categories/NSString+STL.hh
    ${hdrs_dir}/subjective-c/categories/NSURL+IM.hh

    ${hdrs_dir}/subjective-c/classes/AXCoreGraphicsImageRep.h
    ${hdrs_dir}/subjective-c/classes/AXInterleavedImageRep.hh

    ${hdrs_dir}/subjective-c/types.hh
    ${hdrs_dir}/subjective-c/selector.hh
    ${hdrs_dir}/subjective-c/message-args.hh
    ${hdrs_dir}/subjective-c/traits.hh
    ${hdrs_dir}/subjective-c/object.hh
    ${hdrs_dir}/subjective-c/message.hh
    ${hdrs_dir}/subjective-c/namespace-std.hh
    ${hdrs_dir}/subjective-c/namespace-im.hh
    ${hdrs_dir}/subjective-c/subjective-c.hh
    ${hdrs_dir}/subjective-c/appkit.hh
    ${hdrs_dir}/subjective-c/rehash.hh

)

# Project source files
set(srcs

    ${srcs_dir}/categories/NSBitmapImageRep+IM.mm
    ${srcs_dir}/categories/NSColor+IM.mm
    ${srcs_dir}/categories/NSData+IM.mm
    ${srcs_dir}/categories/NSDictionary+IM.mm
    # ${srcs_dir}/categories/NSImage+CGImage.m
    ${srcs_dir}/categories/NSImage+QuickLook.m
    # ${srcs_dir}/categories/NSImage+Resize.m
    # ${srcs_dir}/categories/NSImage+ResizeBestFit.m
    ${srcs_dir}/categories/NSString+STL.mm
    ${srcs_dir}/categories/NSURL+IM.mm

    ${srcs_dir}/classes/AXCoreGraphicsImageRep.m
    ${srcs_dir}/classes/AXInterleavedImageRep.mm

    ${srcs_dir}/src/namespace-std.mm
    ${srcs_dir}/src/selector.mm
    ${srcs_dir}/src/types.mm
    ${srcs_dir}/src/traits.mm

)

# set common link flags
foreach(FLAG IN LISTS ${COMMON_LINK_FLAGS})
    
    set_source_files_properties(
        ${srcs}
        PROPERTIES LINK_FLAGS ${LINK_FLAGS} ${FLAG})
    
endforeach()

# set source properties,
# using /((?P<objc>(ObjC|Objective-C))|(?P<c>(C)))?(?P<plus>(C|\+\+|PP|XX))?/ language-specific stuff
foreach(src_file IN LISTS ${srcs})
    if(${src_file} MATCHES "\.c$")
        set(opts ${C_OPTIONS})
    elseif(${src_file} MATCHES "\.cpp$")
        set(opts ${CXX_OPTIONS})
    elseif(${src_file} MATCHES "\.m$")
        set(opts ${OBJC_OPTIONS})
    elseif(${src_file} MATCHES "\.mm$")
        set(opts ${OBJCXX_OPTIONS})
    else()
        set(opts "")
    endif()
    separate_arguments(${opts})
    if(${src_file} MATCHES "(.*)IO/(.*)")
        # Extra source file props specific to Image IO code compilation
        foreach(extra_opt IN LISTS ${IO_EXTRA_OPTIONS})
            list(APPEND opts ${extra_opt})
        endforeach()
    endif()
    separate_arguments(${opts})
    foreach(opt IN LISTS ${opts})
        get_source_file_property(existant_compile_flags ${src_file} COMPILE_FLAGS)
        set_source_files_properties(${src_file}
            PROPERTIES COMPILE_FLAGS ${existant_compile_flags} ${opt})
    endforeach()
endforeach()

IF(APPLE)
    # Right now there are no non-Apple options that work
    FIND_LIBRARY(SYSTEM_LIBRARY System)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(FOUNDATION_LIBRARY Foundation)
    FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation)
    FIND_LIBRARY(QUICKLOOK_LIBRARY QuickLook)
    
    MARK_AS_ADVANCED(SYSTEM_LIBRARY
                     COCOA_LIBRARY
                     FOUNDATION_LIBRARY
                     COREFOUNDATION_LIBRARY
                     QUICKLOOK_LIBRARY)
    
    SET(EXTRA_LIBS ${EXTRA_LIBS}
        ${SYSTEM_LIBRARY}
        ${COCOA_LIBRARY}
        ${FOUNDATION_LIBRARY}
        ${COREFOUNDATION_LIBRARY}
        ${QUICKLOOK_LIBRARY})
    
ENDIF(APPLE)

add_definitions(
    ${OBJCXX_OPTIONS}
    -Wno-nullability-completeness
    -DWITH_SCHEMA
    -O3 -funroll-loops -mtune=native
    -fstrict-aliasing)
