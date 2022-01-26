cmake_minimum_required(VERSION 3.21)
project(langron)

set(CMAKE_CXX_STANDARD 20)

find_package(LLVM REQUIRED CONFIG)


include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} support core irreader passes target codegen mc mcparser option transformutils)

add_executable(langron main.cpp lexer/lexer.cpp lexer/lexer.h lexer/token.h ast/ExprAST.h ast/NumberExprAST.h ast/VariableExprAST.h ast/BinaryExprAST.h ast/CallExprAST.h ast/PrototypeAST.h ast/FunctionAST.h parser/parser.cpp parser/parser.h util/bin_op.h util/logger.h generator/generator.cpp generator/generator.h ast/IfExprAST.h ast/ForExprAST.h ast/CharacterExprAST.h ast/UnaryExprAST.h ast/BlockExprAST.h ast/MutExprAST.h ast/ReturnExprAST.h types/type_system.cpp types/type_system.h)
target_include_directories(langron PRIVATE .)
target_link_libraries(langron ${llvm_libs})