# Install script for directory: /Users/fish/Dropbox/libimread/deps

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/fish/Dropbox/libimread/xcode/fmwk/deps/crossguid/cmake_install.cmake")
  include("/Users/fish/Dropbox/libimread/xcode/fmwk/deps/docopt/cmake_install.cmake")
  include("/Users/fish/Dropbox/libimread/xcode/fmwk/deps/imagecompression/cmake_install.cmake")
  include("/Users/fish/Dropbox/libimread/xcode/fmwk/deps/iod/cmake_install.cmake")
  include("/Users/fish/Dropbox/libimread/xcode/fmwk/deps/MABlockClosure/cmake_install.cmake")
  include("/Users/fish/Dropbox/libimread/xcode/fmwk/deps/SG14/cmake_install.cmake")
  include("/Users/fish/Dropbox/libimread/xcode/fmwk/deps/SSZipArchive/cmake_install.cmake")

endif()

