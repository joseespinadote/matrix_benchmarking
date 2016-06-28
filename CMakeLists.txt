cmake_minimum_required (VERSION 3.0)

add_executable(main main.cpp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

if(MATH_ENGINE STREQUAL "boost")
    message(STATUS "Using boost as math engine")
    add_definitions(-DMATH_ENGINE=1)
elseif(MATH_ENGINE STREQUAL "eigen")
    message(STATUS "Using Eigen as math engine")
    add_definitions(-DMATH_ENGINE=2)
    include_directories("../Eigen")
elseif(MATH_ENGINE STREQUAL "arma")
    message(STATUS "Using Armadillo as math engine")
    add_definitions(-DMATH_ENGINE=3)
    find_package(Armadillo REQUIRED)
    include_directories(${ARMADILLO_INCLUDE_DIRS})
    target_link_libraries(main ${ARMADILLO_LIBRARIES})
else()
    message(FATAL_ERROR "A valid math engine is required. boost, arma or eigen are availaible at this version")
endif()

