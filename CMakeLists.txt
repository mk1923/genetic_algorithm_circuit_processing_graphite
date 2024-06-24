cmake_minimum_required(VERSION 3.10)

# set the project name
project(Genetic_Algorithm)

find_package(OpenMP REQUIRED)
if(OpenMP_C_FOUND)
    message(STATUS "Found OpenMP")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_C_LIBRARIES "${CMAKE_C_LIBRARIES} ${OpenMP_C_LIBRARIES}")
endif()

if(OpenMP_CXX_FOUND)
    message(STATUS "Found OpenMP CXX")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_CXX_LIBRARIES "${CMAKE_CXX_LIBRARIES} ${OpenMP_CXX_LIBRARIES}")
endif()

# set the include path
include_directories(include)

# add the main code
add_subdirectory(src)

# add the tests
include(CTest)
enable_testing()
add_subdirectory(tests)


