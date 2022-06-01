cmake_minimum_required( VERSION 3.4.3 )

project( 3DCRUST_Sofistik_Post_Processor CXX )

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

set(SOURCES src/CDB_Reader.cpp)
set(SOFISTIK_DIR "C:\\Program Files\\SOFiSTiK\\2020\\SOFiSTiK 2020")
set(SOFISTIK_LIB_DIR "${SOFISTIK_DIR}\\interfaces\\64bit")
set(SOFISTIK_INCLUDE_DIR "${SOFISTIK_DIR}\\interfaces\\examples\\c++")

include_directories(inc)
include_directories(${SOFISTIK_INCLUDE_DIR})

OPTION(3DCRUST_ENABLE_PYBIND11 "Build Python bindings" ON)


# Directories to include header files from
include_directories( external/vtu11/vtu11 )

# If compiler is g++: Enable further  warnings and treat all warnings as errors. fPIC stands for position independent code.
if( CMAKE_COMPILER_IS_GNUCXX )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Werror -fPIC" )
endif( CMAKE_COMPILER_IS_GNUCXX )

add_subdirectory( external/gotraceit )

# ----------------- Set up install paths -------------------------
# Changes the default install path to build/install, assuming build is the project directory
if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
endif( )

# This will add the install path to the rpath of the installed binaries to make dynamic linking work
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}" )


# ----------------- Set up python bindings ------------------------
IF(${3DCRUST_ENABLE_PYBIND11} STREQUAL "ON")
MESSAGE(STATUS "Enabling pybind11 support")
set(PYBIND11_CPP_STANDARD -std=c++14)

# python extension (containing wrapping code and python scipts)
add_subdirectory( extensions/python_extension )

file( GLOB SOURCE_FILES src/*.cpp)
file( GLOB MAIN_FILES main/*.cpp)
file( GLOB HEADER_FILES inc/*.h* )

ENDIF()


# ----------------- Set up CDB_Reader library -------------------------
file( GLOB SOURCE_FILES src/*.cpp)
file( GLOB MAIN_FILES main/*.cpp)
file( GLOB HEADER_FILES inc/*.h* )
# Add library from the collected source files. The headers are given so visual studio displays them
add_library( CDB_Reader SHARED ${SOURCE_FILES} ${HEADER_FILES} ) 


# Add iclude depencency for vtu11
target_include_directories(CDB_Reader PUBLIC ./external/vtu11)

# Add iclude depencency for gotraceit
target_include_directories(CDB_Reader PUBLIC ./external/gotraceit)


# Add include depencency to inc/ folder. 
target_include_directories( CDB_Reader PUBLIC inc )

# specify the relative path the shared library object shall be installed to
if( WIN32 )
  install( TARGETS CDB_Reader RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
else( )
  install( TARGETS CDB_Reader LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} )
endif( )

# ------------------- Set up CDB_Reader_main ---------------------------

# Add an executable and link to the CDB_Reader afterwards
add_executable( 3DCRUST ${MAIN_FILES} )

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(3DCRUST  ${VTK_LIBRARIES} CDB_Reader )

#target_link_libraries( CDB_Reader_main CDB_Reader )

#important for the excutable to work without library loading error
set_target_properties(  3DCRUST PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${SOFISTIK_DIR}")

# Specify the relative path the CDB_Reader_main shall be installed to
install( TARGETS 3DCRUST RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )






