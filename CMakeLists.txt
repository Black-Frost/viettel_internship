cmake_minimum_required(VERSION 3.0.0)
cmake_policy(SET CMP0054 NEW)

set (PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_NAME "Week1")
set(OUTPUT_FOLDER "bin")

# output file exe location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../${OUTPUT_FOLDER})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
# using Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
# using g++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
# using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

set(DEBUG_FOLDER "bin") # vs only

# the included folder for <foo.h>
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${SOURCE_FOLDER}/*.cpp")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${SOURCE_FOLDER}/*.c")
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${INCLUDE_FOLDER}/*.h" "${INCLUDE_FOLDER}/*.hpp")
file(GLOB_RECURSE RESOURCE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${OTHER_FILES_FOLDER}/*.${OTHER_FILES_EXTENSION}")

# set(ALL_FILES ${SOURCES}\  ${HEADERS}\  ${RESOURCE})

# using filter in VS
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# tạo folder cho project
foreach(FILE ${ALL_FILES})
  get_filename_component(PARENT_DIR "${FILE}" PATH)

  # skip src or include and changes /'s to \\'s
  string(REGEX REPLACE "(\\./)?(${SOURCE_FOLDER}|${INCLUDE_FOLDER}|${OTHER_FILES_FOLDER})/?" "" GROUP "${PARENT_DIR}")
  string(REPLACE "/" "\\" GROUP "${GROUP}")

  # group into "Source Files" and "Header Files"
  if ("${FILE}" MATCHES ".*\\.cpp")
    set(GROUP "Source Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.c")
    set(GROUP "Source Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.h")
    set(GROUP "Header Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.${OTHER_FILES_EXTENSION}")
    set(GROUP "Resource Files\\${GROUP}")
  endif()

  source_group("${GROUP}" FILES "${FILE}")
endforeach()

# thư mục khi chạy Debug
# set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${DEBUG_FOLDER}")
endif()

# WEEK 1
add_executable(calculator week1/calculator.c)
add_executable(arrays week1/arrays.cpp)
add_executable(thuvien week1/thuvien.c)
add_executable(bee week1/bee.cpp)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
add_executable(calculator_ASM week1/calculator_ASM.cpp)
endif()

# WEEK 3
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
add_library(sss SHARED week3/sss.cpp)
add_executable(trojan_sss week3/trojan_sss.cpp)
# target_link_libraries(trojan_sss sss)
endif()

# WEEK 4
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
add_executable(IsDebuggerPresent week4/Anti-Debug/IsDebuggerPresent.cpp)
add_executable(PEBBeingDebugged week4/Anti-Debug/PEBBeingDebugged.cpp)
add_executable(PEBNtGlobalFlag week4/Anti-Debug/PEBNtGlobalFlag.cpp)
add_executable(PEBProcessHeap week4/Anti-Debug/PEBProcessHeap.cpp)
add_executable(CheckRemoteDebuggerPresent week4/Anti-Debug/CheckRemoteDebuggerPresent.cpp)
add_executable(NtQueryInformationProcess week4/Anti-Debug/NtQueryInformationProcess.cpp)
add_executable(SEH week4/Anti-Debug/SEH.cpp)
add_executable(VEH week4/Anti-Debug/VEH.cpp)
endif()

# WEEK 6
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
add_executable(freenki_hacked week6/hacked.cpp)
endif()

# vim: set noautoindent
