# vim: set et ts=2 sw=2 ft=cmake nospell:
## CMake settings

cmake_minimum_required (VERSION 2.6)
project(MTE7102)

if(NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/src)
  message(FATAL_ERROR "The dependency repositories (GLFW, etc.) are missing! "
    "You probably did not clone the project with --recursive. It is possible to recover "
    "by calling \"git submodule update --init --recursive\"")
endif()

set(CMAKE_BUILD_TYPE "Debug")
include(CheckCXXCompilerFlag)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)

if(APPLE)
  # Try to auto-detect a suitable SDK
  execute_process(COMMAND bash -c "xcodebuild -version -sdk | grep MacOSX | grep Path | head -n 1 | cut -f 2 -d ' '" OUTPUT_VARIABLE CMAKE_OSX_SYSROOT)
  string(REGEX REPLACE "(\r?\n)+$" "" CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}")
  string(REGEX REPLACE "^.*X([0-9.]*).sdk$" "\\1" CMAKE_OSX_DEPLOYMENT_TARGET "${CMAKE_OSX_SYSROOT}")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # Enable C++11 mode on GCC / Clang
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-anonymous-struct -Wno-c99-extensions -Wno-nested-anon-types")
  endif()
endif()

# Turn off unneeded GLFW things
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_INSTALL OFF CACHE BOOL " " FORCE)
set(GLFW_INSTALL OFF CACHE BOOL " " FORCE)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Quench annoying deprecation warnings when compiling GLFW on OSX
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
endif()

# Output various settings
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Use package PkgConfig
find_package(PkgConfig REQUIRED)

# Use package OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    message(STATUS "Found OpenGL: ${OPENGL_LIBRARY}" )
endif(OPENGL_FOUND)

# Specify additional subdirectories (needs CMakeLists.txt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw)

# Specify additional include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/eigen)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/nanovg/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/spdlog/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/pugixml)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/jinja2cpplight)

# Set source files
set(SOURCES
  # Merge GLFW into the source
  util.hpp util.cpp
  property.hpp
  glshaderobject.hpp glshaderobject.cpp
  glshader.hpp glshader.cpp
  clickablelabel.hpp clickablelabel.cpp
  directpopup.hpp directpopup.cpp
  connector.hpp connector.cpp
  sink.hpp sink.cpp
  source.hpp source.cpp
  graphnodelink.hpp graphnodelink.cpp
  graphnode.hpp graphnode.cpp
  outputgraphnode.hpp outputgraphnode.cpp
  genericgraphnode.hpp genericgraphnode.cpp
  graph.hpp graph.cpp
  qce.hpp  qce.cpp
  main.hpp main.cpp
)

# Specify additional linking directories
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)

# Resource file (icons etc.)
set(EXTRA_SOURCE "")
if (APPLE)
  set(EXTRA_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/../resources/im.icns"
    "${CMAKE_CURRENT_SOURCE_DIR}/../resources/data/sphere_tracer.fs"
    "${CMAKE_CURRENT_SOURCE_DIR}/../resources/data/objects/sphere.fs.xml"
    "${CMAKE_CURRENT_SOURCE_DIR}/../resources/data/objects/box.fs.xml"
  )
endif()


# Add external libraries (which have no CMakeLists file)
set(EXTERNAL_SOURCE
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/jinja2cpplight/src/stringhelper.h  ${CMAKE_CURRENT_SOURCE_DIR}/ext/jinja2cpplight/src/stringhelper.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/jinja2cpplight/src/Jinja2CppLight.h ${CMAKE_CURRENT_SOURCE_DIR}/ext/jinja2cpplight/src/Jinja2CppLight.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/pugixml/src/pugixml.hpp ${CMAKE_CURRENT_SOURCE_DIR}/ext/pugixml/src/pugiconfig.hpp ${CMAKE_CURRENT_SOURCE_DIR}/ext/pugixml/src/pugixml.cpp
)


# Specify executable file for output
add_executable(mte7102 MACOSX_BUNDLE
    ${SOURCES}
    ${EXTRA_SOURCE}
    ${EXTERNAL_SOURCE}
)

# Link needed libraries
target_link_libraries(mte7102
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARIES}
    libnanogui.dylib
)

# Set output directory of executable
set_target_properties(mte7102 PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin
    OUTPUT_NAME "MTE7102"
)

if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
  # Link-time code generation (only for shared library)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set_property(TARGET mte7102 APPEND_STRING PROPERTY COMPILE_FLAGS "-fvisibility=hidden ")

    # Check for Link Time Optimization support
    CHECK_CXX_COMPILER_FLAG("-flto" HAS_LTO_FLAG)
    if (HAS_LTO_FLAG)
      set_property(TARGET mte7102 APPEND_STRING PROPERTY COMPILE_FLAGS "-flto ")
    endif()
  endif()
endif()


# Merge GLFW
set_property(TARGET mte7102 APPEND PROPERTY COMPILE_DEFINITIONS "_GLFW_BUILD_DLL ")

if (APPLE)
  # Build an application bundle on OSX
  set_target_properties(mte7102 PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "MTE7102")
  set_target_properties(mte7102 PROPERTIES MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.im.mte7102")
  set_target_properties(mte7102 PROPERTIES MACOSX_BUNDLE_ICON_FILE im.icns)
  set_target_properties(mte7102 PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/../resources/MacOSXBundleInfo.plist.in)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/../resources/im.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/../resources/data/sphere_tracer.fs PROPERTIES MACOSX_PACKAGE_LOCATION Resources/data/)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/../resources/data/objects/sphere.fs.xml PROPERTIES MACOSX_PACKAGE_LOCATION Resources/data/objects/)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/../resources/data/objects/box.fs.xml PROPERTIES MACOSX_PACKAGE_LOCATION Resources/data/objects/)
else()
  # Insulate from a few types of ABI changes by statically linking against libgcc and libstdc++
  set_target_properties(mte7102 PROPERTIES LINK_FLAGS "-static-libgcc")
  #set_target_properties(mte7102 PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
endif()

if (UNIX AND NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
  add_custom_command(TARGET mte7102 POST_BUILD COMMAND strip $<TARGET_FILE:mte7102>)
endif()
