//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_EXPRAST_H
#define LANGRON_EXPRAST_H

#include "generator/generator.h"


class ExprAST {
public:
    virtual ~ExprAST() = default;
    virtual llvm::Value *codegen() = 0;
};

#endif //LANGRON_EXPRAST_H
