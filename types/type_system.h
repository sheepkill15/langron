//
// Created by simi on 1/26/22.
//

#ifndef LANGRON_TYPE_SYSTEM_H
#define LANGRON_TYPE_SYSTEM_H


#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

class type_system {
public:
    type_system() = delete;

    static llvm::Type* generate_type(const std::string& type);
    static llvm::Value* generate_cast(llvm::Value* srcValue, llvm::Type* destType, const std::string& variableName);
    static llvm::Value* depointify(llvm::Value* srcValue, const std::string& variableName);
};


#endif //LANGRON_TYPE_SYSTEM_H
