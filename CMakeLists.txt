cmake_minimum_required(VERSION 2.8)
project(SymbRegExercise)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

#set(SOURCE_FILES pithagorasExplicitSymbolicRegression.cpp PithagoraSymbRegEvalOp.cpp PithagoraSymbRegEvalOp.h)
set(SOURCE_FILES implicit_functions/ImplicitEvaluationWithPartialDerivatives.cpp implicit_functions/ParetoFrontier.cpp implicit_functions/ParetoFrontier.h implicit_functions/parameter_nodes/ParametersBuilder.cpp implicit_functions/parameter_nodes/ParametersBuilder.h implicit_functions/parameter_nodes/ParametersBuilderTest.cpp)

include_directories(../../)
add_executable(SymbRegExercise ${SOURCE_FILES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(SymbRegExercise /usr/local/lib/libecf.so)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(SymbRegExercise /usr/local/lib/libecf.so)
endif()