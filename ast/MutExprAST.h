//
// Created by simi on 1/23/22.
//

#ifndef LANGRON_MUTEXPRAST_H
#define LANGRON_MUTEXPRAST_H


#include "ExprAST.h"

class MutExprAST : public ExprAST {
    std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> var_names;

public:
    explicit MutExprAST(std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> var_names)
    :var_names(std::move(var_names)) {};

    llvm::Value *codegen() override {
        std::vector<llvm::AllocaInst*> oldBindings;
        auto theFunction = generator::builder->GetInsertBlock()->getParent();

        for(auto & var_name : var_names) {
            const auto& varName = var_name.first;
            auto init = var_name.second.get();
            llvm::Value* initVal;
            if(init) {
                initVal = init->codegen();
                if(!initVal) {
                    return nullptr;
                }
            } else {
                initVal = llvm::ConstantFP::get(*generator::theContext, llvm::APFloat(0.0));
            }
            auto alloca = generator::CreateEntryBlockAlloca(theFunction, varName);
            generator::builder->CreateStore(initVal, alloca);
            if(generator::namedValues.contains(varName)) {
                oldBindings.push_back(generator::namedValues.at(varName));
            }
            generator::namedValues[varName] = alloca;
        }
        return nullptr;
    }
};


#endif //LANGRON_MUTEXPRAST_H
