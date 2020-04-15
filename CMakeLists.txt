cmake_minimum_required(VERSION 2.8)
project(SymbRegExercise)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        implicit_functions/ImplicitEvaluationWithPartialDerivatives.cpp
        implicit_functions/ImplicitEvaluationWithPartialDerivatives.h
        implicit_functions/ParetoFrontier.cpp
        implicit_functions/ParetoFrontier.h
        implicit_functions/parameter_nodes/ParametersBuilder.cpp
        implicit_functions/parameter_nodes/ParametersBuilder.h
        implicit_functions/unordered/UnorderedDatasetPreparation.cpp
        implicit_functions/unordered/UnorderedDatasetPreparation.h
        direct_methods/SimpleStdevEvaluator.cpp
        direct_methods/SimpleStdevEvaluator.h
        utils/Point.cpp
        utils/Point.h
        utils/MathFunctions.cpp
        utils/MathFunctions.h
        direct_methods/MultipleTreeEvaluator.cpp
        direct_methods/MultipleTreeEvaluator.h
        implicit_functions/unordered/Unordered2DImplicitEvaluation.cpp
        implicit_functions/unordered/Unordered2DImplicitEvaluation.h
        utils/Cone.cpp
        utils/Cone.h
        implicit_functions/unordered/UnorderedMultiDimImplicitEvaluation.cpp
        implicit_functions/unordered/UnorderedMultiDimImplicitEvaluation.h
        utils/MultiDimEllipse.cpp
        utils/MultiDimEllipse.h
        implicit_functions/SymbolicRegressionUtil.cpp
        implicit_functions/SymbolicRegressionUtil.h
        implicit_functions/unordered/UnorderedMultiDimMultiGenEval.cpp
        implicit_functions/unordered/UnorderedMultiDimMultiGenEval.h
        utils/primitives/Exp.h
        utils/primitives/Sqrt.h
        utils/primitives/Tanh.h
        utils/primitives/Square.h
        executables/BenchmarkTesting.cpp)

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

include_directories(../../)
add_executable(SymbRegExercise ${SOURCE_FILES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(SymbRegExercise /usr/local/lib/libecf.so)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(SymbRegExercise /usr/local/lib/libecf.so)
endif()