//
// Created by simi on 1/29/22.
//

#ifndef LANGRON_BINARYOPERATIONS_H
#define LANGRON_BINARYOPERATIONS_H


#include <string>
#include <map>
#include <llvm/IR/Value.h>

class BinaryOperations {
public:
    BinaryOperations() = delete;
    static int getTokPrecedence(const std::string& token);
    static std::map<std::string, int> binOpPrecedence;

    static llvm::Value* CreateAdd(llvm::Value* lhs, llvm::Value* rhs);
    static llvm::Value* CreateSub(llvm::Value* lhs, llvm::Value* rhs);
    static llvm::Value* CreateMul(llvm::Value* lhs, llvm::Value* rhs);
    static llvm::Value* CreateLT(llvm::Value* lhs, llvm::Value* rhs);
    static llvm::Value* CreateEQ(llvm::Value* lhs, llvm::Value* rhs);
    static llvm::Value* CreateGT(llvm::Value* lhs, llvm::Value* rhs);
};


#endif //LANGRON_BINARYOPERATIONS_H
