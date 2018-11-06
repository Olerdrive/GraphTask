cmake_minimum_required(VERSION 3.12)
project(GraphTask)

set(CMAKE_CXX_STANDARD 14)


find_package(Boost 1.67.0 COMPONENTS graph thread REQUIRED)

if(Boost_FOUND)

    include_directories(${Boost_INCLUDE_DIRS})

endif()

add_executable(GraphTask main.cpp graphboost.cpp graphmatrix.cpp)

if(Boost_FOUND)

    target_link_libraries(GraphTask ${Boost_LIBRARIES})

endif()