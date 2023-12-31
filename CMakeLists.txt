cmake_minimum_required(VERSION 3.16)
project(fruitlang)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 20)

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

set(sources ${sources}
        Lexer.cpp
        Lexer.h
        token.h
        ast/ast.cpp
        ast/ast.h
        ast/expr.cpp
        ast/expr.h
        ast/module.cpp
        ast/module.h
        ast/block.cpp
        ast/block.h
        ast/binary_operations.cpp
        ast/binary_operations.h
        ast/callable.cpp
        ast/callable.h
        ast/ast_Type.cpp
        ast/ast_Type.h
        ast/call.cpp
        ast/call.h
        ast/unary_operations.cpp
        ast/unary_operations.h
        Parser.cpp
        Parser.h
        ast/Literal.cpp
        ast/Literal.h
        utils.cpp
        utils.h
        codegen.cpp
        codegen.h
        ast/access.cpp
        ast/access.h
        Typechecker.cpp
        Typechecker.h
        includes.h
        ast/ast_if.cpp ast/ast_if.h)

# Now build our tools
add_executable(fruitlang main.cpp ${sources})
add_executable(test_fruitlang test.cpp ${sources})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader passes orcjit x86codegen x86asmparser)

# Link against LLVM libraries
target_link_libraries(fruitlang PUBLIC ${llvm_libs})
target_compile_options(fruitlang PRIVATE -Werror -Wall -Wextra -pedantic -Wconversion)

target_link_libraries(test_fruitlang PUBLIC ${llvm_libs})
target_compile_options(test_fruitlang PRIVATE -Werror -Wall -Wextra -pedantic -Wconversion)

