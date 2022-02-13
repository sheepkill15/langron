//
// Created by simi on 1/23/22.
//

#ifndef LANGRON_MUTEXPRAST_H
#define LANGRON_MUTEXPRAST_H


#include "ExprAST.h"
#include "TypeExprAST.h"

class MutExprAST : public ExprAST {
    std::vector<std::tuple<std::string, std::unique_ptr<ExprAST>, std::unique_ptr<TypeExprAST>>> var_names;

public:
    explicit MutExprAST(std::vector<std::tuple<std::string, std::unique_ptr<ExprAST>, std::unique_ptr<TypeExprAST>>> var_names)
    :var_names(std::move(var_names)) {};

    llvm::Value *codegen() override {
        std::vector<llvm::AllocaInst*> oldBindings;
        auto theFunction = generator::builder->GetInsertBlock()->getParent();

        for(auto & var_name : var_names) {
            const auto& varName = std::get<0>(var_name);
            auto &init = std::get<1>(var_name);
            llvm::Value* initVal;
            llvm::Type* type;
            if(init) {
                initVal = init->codegen();
                if(!initVal) {
                    return nullptr;
                }
                type = initVal->getType();
            } else {
                initVal = llvm::ConstantFP::get(*generator::theContext, llvm::APFloat(0.0));
            }
            auto& typeNode = std::get<2>(var_name);
            if(typeNode != nullptr) {
                type = typeNode->typegen();
                initVal = type_system::generate_cast(initVal, type, "");

            }
            llvm::AllocaInst* alloca = generator::CreateEntryBlockAlloca(theFunction, varName, type);
            if(!type->isArrayTy()) {
                generator::builder->CreateStore(initVal, alloca);
            }
            if(generator::namedValues.contains(varName)) {
                oldBindings.push_back(generator::namedValues.at(varName));
            }
            generator::namedValues[varName] = alloca;
        }
        return nullptr;
    }
};


#endif //LANGRON_MUTEXPRAST_H
