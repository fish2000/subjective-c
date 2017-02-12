# Author: Alexander Böhn (with Félix C. Morency)
# © 2011.10 -- GPL, Motherfuckers

# Set the following variable
#   subjective-c_FOUND: True if the library has been found
#   subjective-c_include_dir: Path to header files

# Assure that the library has not already been found
if(NOT ${subjective-c_FOUND})
    set(lib_name subjective-c)
    
    # Find the include directory
    find_path(${lib_name}_include_dir NAMES ${lib_name}.hpp ${lib_name}.hpp.in
        PATHS ${${lib_name}_DIR}
        PATH_SUFFIXES /../include/subjective-c)
    
    # Standard work to make sure the different variables have been set. This
    # command will set the subjectivec_FOUND variable.
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(${lib_name} DEFAULT_MSG
        ${lib_name}_include_dir)
    
    # If the target have not already been added and if the project calling this
    # file is not the subjectivec CMakeLists.txt, add the library to the
    # build chain.
    if(NOT TARGET ${lib_name} AND
        NOT ${PROJECT_NAME} STREQUAL ${lib_name})
        add_subdirectory(${${lib_name}_DIR}/../
            ${CMAKE_CURRENT_BINARY_DIR}/lib/${lib_name})
    endif()
endif()

