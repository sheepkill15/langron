//
// Created by simi on 1/29/22.
//

#ifndef LANGRON_TYPEEXPRAST_H
#define LANGRON_TYPEEXPRAST_H


#include "ExprAST.h"
#include "NumberExprAST.h"

class TypeExprAST {
    std::string typeName;
    bool isReference;
    std::unique_ptr<ExprAST> arraySize;
public:
    bool isArray;
    TypeExprAST(std::string type_Name, bool is_Reference, bool is_Array, std::unique_ptr<ExprAST> array_size)
    : typeName(std::move(type_Name)), isReference(is_Reference), isArray(is_Array), arraySize(std::move(array_size)) {};

    llvm::Type* typegen() {
        auto type = type_system::generate_type(typeName);
        if(isArray) {
            if(auto *intVal = dynamic_cast<NumberExprAST*>(arraySize.get())) {
                auto arrType = llvm::ArrayType::get(type, (uint64_t)intVal->value);
                return arrType;
            }
        }
        if(isReference) {
            return llvm::PointerType::getUnqual(type);
        }
        return type;
    }
};


#endif //LANGRON_TYPEEXPRAST_H
