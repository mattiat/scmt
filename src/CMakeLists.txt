# Name        : TranscriptionNetwork
# Author      : Mattia Tomasoni
# Institution : University of Leeds

###############################################################################
# COMPILER OPTIONS
###############################################################################

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_C_FLAGS "-O3 ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 ")
set(CMAKE_C_FLAGS_RELEASE "-O3 ")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")

###############################################################################
# EXTERNAL LIBRARIES
###############################################################################

include_directories(${CMAKE_CURRENT_BINARY_DIR})
find_package(Boost 1.36.0 COMPONENTS thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

###############################################################################
# COMPILE
###############################################################################

include_directories(.)
add_library( transnetlib 
  STATIC DataGraph.cpp Edge.cpp Gene.cpp main.cpp Network.cpp Signal.cpp
)

add_executable(TranscriptionNetwork 
  main.cpp
)

target_link_libraries(TranscriptionNetwork 
  transnetlib
  ${Boost_THREAD_LIBRARY}
)

install(
  TARGETS TranscriptionNetwork 
  EXPORT TranscriptionNetwork 
  DESTINATION ./bin
)
