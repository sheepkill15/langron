//
// Created by simi on 1/29/22.
//

#ifndef LANGRON_ARRAYACCESSEXPRAST_H
#define LANGRON_ARRAYACCESSEXPRAST_H


#include "ExprAST.h"
#include "NumberExprAST.h"

class ArrayAccessExprAST : public ExprAST {
    std::vector<std::unique_ptr<ExprAST>> indexes;
public:
    std::string arrName;
    ArrayAccessExprAST(std::string arrName, std::vector<std::unique_ptr<ExprAST>> indexes) : arrName(std::move(arrName)), indexes(std::move(indexes)) {};
    llvm::Value* codegen() override {
        if(!generator::namedValues.contains(arrName)) {
            return logError<llvm::Value*>("Unknown variable name");
        }
        llvm::Value *v = generator::namedValues.at(arrName);
        auto zero = llvm::ConstantInt::get(*generator::theContext, llvm::APInt(64, 0, true));
        for(auto& expr : indexes) {
            auto indexV = expr->codegen();
            auto intIndex = type_system::generate_cast(indexV, llvm::Type::getInt32Ty(*generator::theContext), "casttmp");
            v = generator::builder->CreateGEP(v->getType()->getContainedType(0), v,
                                                {zero, intIndex}, "reftmp");
        }
        return v;
    }
};


#endif //LANGRON_ARRAYACCESSEXPRAST_H
