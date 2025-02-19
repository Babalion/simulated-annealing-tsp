cmake_minimum_required(VERSION 2.6)
project(SA)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DVERBOSE_MODE -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
set(BUILD_ARCH "-m64")

find_package( OpenCV REQUIRED )

add_executable(sa src/main.cpp src/tsp.cpp )

target_link_libraries( sa
    ${OpenCV_LIBS}
)
