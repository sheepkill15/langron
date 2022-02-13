//
// Created by simi on 1/24/22.
//

#ifndef LANGRON_RETURNEXPRAST_H
#define LANGRON_RETURNEXPRAST_H


#include "ExprAST.h"

class ReturnExprAST : public ExprAST {
    std::unique_ptr<ExprAST> expression;
public:
    explicit ReturnExprAST(std::unique_ptr<ExprAST> expr) : expression(std::move(expr)) {};

    llvm::Value* codegen() override {
        auto val = expression->codegen();

        auto theFunction = generator::builder->GetInsertBlock()->getParent();
        val = type_system::generate_cast(val, theFunction->getReturnType(), "");
        generator::builder->CreateRet(val);
        return val;
    }
};


#endif //LANGRON_RETURNEXPRAST_H
