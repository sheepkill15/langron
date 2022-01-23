//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_PARSER_H
#define LANGRON_PARSER_H


#include <memory>
#include "ast/ExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/FunctionAST.h"

class parser {
public:
    parser() = delete;

    static int getNextToken();
    static std::unique_ptr<ExprAST> parsePrimary();

    static std::unique_ptr<ExprAST> parseNumberExpr();
    static std::unique_ptr<ExprAST> parseParenExpr();
    static std::unique_ptr<ExprAST> parseIdentifierExpr();
    static std::unique_ptr<ExprAST> parseExpression();
    static std::unique_ptr<ExprAST> parseCharacterExpr();
    static std::unique_ptr<ExprAST> parseBinOpRHS(int exprPrec, std::unique_ptr<ExprAST> lhs);
    static std::unique_ptr<ExprAST> parseUnary();

    static std::unique_ptr<PrototypeAST> parsePrototype();
    static std::unique_ptr<FunctionAST> parseDefinition();
    static std::unique_ptr<PrototypeAST> parseExtern();

    static std::unique_ptr<FunctionAST> parseTopLevelExpr();

    static std::unique_ptr<ExprAST> parseIfExpr();
    static std::unique_ptr<ExprAST> parseForExpr();
    static std::unique_ptr<ExprAST> parseBlockExpr();
    static std::unique_ptr<ExprAST> parseReturnExpr();
    static std::unique_ptr<ExprAST> parseMutExpr();

    static void mainLoop();
    static void handleDefinition();
    static void handleExtern();
    static void handleTopLevelExpression();



    inline static int cur_token;
};


#endif //LANGRON_PARSER_H
