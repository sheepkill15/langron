//
// Created by simi on 1/23/22.
//

#ifndef LANGRON_BLOCKEXPRAST_H
#define LANGRON_BLOCKEXPRAST_H


#include "ExprAST.h"

class BlockExprAST : public ExprAST {
    std::vector<std::unique_ptr<ExprAST>> expressions;
    std::unique_ptr<ExprAST> returnExpr;
public:
    explicit BlockExprAST(std::vector<std::unique_ptr<ExprAST>> expressions, std::unique_ptr<ExprAST> returnExpr = nullptr)
        :expressions(std::move(expressions)), returnExpr(std::move(returnExpr)) {};

    llvm::Value* codegen() override {
        llvm::Value* lastExpr = nullptr;
        for(auto& expr : expressions) {
            lastExpr = expr->codegen();
        }
        if(returnExpr) {
            auto returnV = returnExpr->codegen();
            if(returnV) {
                return returnV;
            }
        }
        if(lastExpr) {
            return lastExpr;
        }
        return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(*generator::theContext));
    }
};


#endif //LANGRON_BLOCKEXPRAST_H
