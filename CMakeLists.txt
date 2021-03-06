cmake_minimum_required(VERSION 3.21)
project(langron)

set(CMAKE_CXX_STANDARD 20)


if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(BUILD_FOR_WIN TRUE)
endif()
option(STATIC_BUILD "Build a static binary." ${BUILD_FOR_WIN})

if (STATIC_BUILD)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" CONFIG)
    set(BUILD_SHARED_LIBS OFF)
endif()

set(LLVM_DIR C:\\Program Files\\LLVM)

find_package(LLVM REQUIRED CONFIG)


include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} support core irreader passes target codegen mc mcparser option transformutils)

add_executable(langron main.cpp lexer/lexer.cpp lexer/lexer.h lexer/token.h ast/ExprAST.h ast/NumberExprAST.h ast/VariableExprAST.h ast/BinaryExprAST.h ast/CallExprAST.h ast/PrototypeAST.h ast/FunctionAST.h parser/parser.cpp parser/parser.h util/logger.h generator/generator.cpp generator/generator.h ast/IfExprAST.h ast/ForExprAST.h ast/CharacterExprAST.h ast/UnaryExprAST.h ast/BlockExprAST.h ast/MutExprAST.h ast/ReturnExprAST.h types/type_system.cpp types/type_system.h ast/TypeExprAST.h util/BinaryOperations.cpp util/BinaryOperations.h ast/ArrayAccessExprAST.h ast/StringExprAST.h)
target_include_directories(langron PRIVATE .)
target_link_libraries(langron ${llvm_libs})