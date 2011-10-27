# Name        : TranscriptionNetwork
# Author      : Mattia Tomasoni
# Institution : University of Leeds

###############################################################################
# NAME AND VERSION
###############################################################################

cmake_minimum_required(VERSION 2.8)
project(TranscriptionNetwork)

# VERSION AND NAMESPACES
set(VERSION_MAJOR "0")
set(VERSION_MINOR "00")

###############################################################################
# CONFIG VARIABLES
###############################################################################

set(TEMP_DIR "/usr/not-backed-up/tmp/")
set(WRITABLE_DIR ${TEMP_DIR})
set(MAX_TIME "100.0")
set(TIME_STEP "0.01")

# STORE NEWLY SET VARIABLES IN *.h.cmake FILES
file(
GLOB_RECURSE configurefiles 
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.cmake 
)
foreach(i ${configurefiles})
  string(REPLACE ".cmake" "" myReplFile "${i}")
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${myReplFile}.cmake 
    ${CMAKE_CURRENT_BINARY_DIR}/${myReplFile} @ONLY
  )
endforeach()

###############################################################################
# COMPILE
###############################################################################

add_subdirectory(src)


