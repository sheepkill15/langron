//
// Created by simi on 1/22/22.
//

#include <iostream>
#include "parser.h"
#include "lexer/lexer.h"
#include "ast/NumberExprAST.h"
#include "ast/VariableExprAST.h"
#include "ast/CallExprAST.h"
#include "lexer/token.h"
#include "ast/BinaryExprAST.h"
#include "util/logger.h"
#include "llvm/Support/raw_os_ostream.h"
#include "ast/IfExprAST.h"
#include "ast/ForExprAST.h"
#include "ast/CharacterExprAST.h"
#include "ast/UnaryExprAST.h"
#include "ast/BlockExprAST.h"
#include "ast/MutExprAST.h"
#include "ast/ReturnExprAST.h"
#include "ast/TypeExprAST.h"
#include "ast/ArrayAccessExprAST.h"

int parser::getNextToken() {
    return cur_token = lexer::gettok();
}

std::unique_ptr<ExprAST> parser::parsePrimary() {
    switch (cur_token) {
        case Token::tok_identifier:
            return parseIdentifierExpr();
        case Token::tok_number:
            return parseNumberExpr();
        case '(':
            return parseParenExpr();
        case Token::tok_if:
            return parseIfExpr();
        case Token::tok_for:
            return parseForExpr();
        case '\'':
            return parseCharacterExpr();
        case '{':
            return parseBlockExpr();
        case '}':
            return nullptr;
        case '[':
            return parseArrayExpr();
        case Token::tok_ret:
            return parseReturnExpr();
        case Token::tok_mut:
            return parseMutExpr();
        default:
            return logError<std::unique_ptr<ExprAST>>("unknown token when expecting an expression");
    }
}

std::unique_ptr<ExprAST> parser::parseNumberExpr() {
    auto result = std::make_unique<NumberExprAST>(lexer::state.number_value);
    getNextToken();
    return std::move(result);
}

std::unique_ptr<ExprAST> parser::parseIdentifierExpr() {
    std::string id_name = lexer::state.identifier_str;
    getNextToken();
    if(cur_token == '[') {
        std::vector<std::unique_ptr<ExprAST>> arrExprs;
        while(cur_token == '[' || cur_token == ',') {
            getNextToken();
            auto expr = parseExpression();
            arrExprs.push_back(std::move(expr));
            if(cur_token != ']' && cur_token != ',') {
                return logError<std::unique_ptr<ExprAST>>("required ]");
            }
        }

        if(cur_token != ']') {
            return logError<std::unique_ptr<ExprAST>>("expected ]");
        }
        getNextToken();
        return std::make_unique<ArrayAccessExprAST>(id_name, std::move(arrExprs));
    }
    if(cur_token != '(') {
        return std::make_unique<VariableExprAST>(id_name);
    }
    getNextToken();
    std::vector<std::unique_ptr<ExprAST>> args;
    while(cur_token != ')') {
        if(auto arg = parseExpression()) {
            args.push_back(std::move(arg));
        }
        else {
            return nullptr;
        }
        if(cur_token == ')') break;
        if(cur_token != ',') {
            return logError<std::unique_ptr<ExprAST>>("Expected ')' or ',' in argument list");
        }
        getNextToken();
    }
    getNextToken();
    return std::make_unique<CallExprAST>(id_name, std::move(args));
}

std::unique_ptr<ExprAST> parser::parseParenExpr() {
    getNextToken();
    auto V = parseExpression();
    if(!V) {
        return nullptr;
    }
    if(cur_token != ')') {
        return logError<std::unique_ptr<ExprAST>>("expected ')'");
    }
    getNextToken();
    return V;
}

std::unique_ptr<ExprAST> parser::parseExpression() {
    auto lhs = parseUnary();
    if(!lhs) {
        return nullptr;
    }
    return parseBinOpRHS(0, std::move(lhs));
}

std::unique_ptr<ExprAST> parser::parseBinOpRHS(int exprPrec, std::unique_ptr<ExprAST> lhs) {
    while(true) {
        auto binOp = lexer::state.identifier_str;
        int tokPrec = BinaryOperations::getTokPrecedence(binOp);
        if(!isascii(cur_token)) {
            tokPrec = -1;
        }
        if(tokPrec < exprPrec) {
            return lhs;
        }
        getNextToken();

        auto rhs = parseUnary();
        if(!rhs) {
            return nullptr;
        }
        int nextPrec = BinaryOperations::getTokPrecedence(lexer::state.identifier_str);
        if(!isascii(cur_token)) {
            nextPrec = -1;
        }
        if(tokPrec < nextPrec) {
            rhs = parseBinOpRHS(tokPrec + 1, std::move(rhs));
            if(!rhs) {
                return nullptr;
            }
        }
        lhs = std::make_unique<BinaryExprAST>(binOp, std::move(lhs), std::move(rhs));
    }
}

std::unique_ptr<PrototypeAST> parser::parsePrototype() {
    std::string fn_name = lexer::state.identifier_str;
    unsigned kind;
    unsigned binaryPrecedence = 30;

    switch(cur_token) {
        default:
            return logError<std::unique_ptr<PrototypeAST>>("Expected function name in prototype");
        case Token::tok_identifier:
            fn_name = lexer::state.identifier_str;
            kind = 0;
            getNextToken();
            break;
        case Token::tok_unary:
            getNextToken();
            if(!isascii(cur_token)) {
                return logError<std::unique_ptr<PrototypeAST>>("Expected unary operator");
            }
            fn_name = "unary";
            fn_name += lexer::state.identifier_str;
            kind = 1;
            getNextToken();
            break;
        case Token::tok_binary:
            getNextToken();
            if(!isascii(cur_token)) {
                return logError<std::unique_ptr<PrototypeAST>>("Expected binary operator");
            }
            fn_name = "binary";
            fn_name += lexer::state.identifier_str;
            kind = 2;
            getNextToken();

            if(cur_token == Token::tok_number) {
                if(lexer::state.number_value < 1 || lexer::state.number_value > 100) {
                    return logError<std::unique_ptr<PrototypeAST>>("Precedence must be in range [1, 100]");
                }
                binaryPrecedence = (unsigned)lexer::state.number_value;
                getNextToken();
            }
            break;
    }

    if(cur_token != '(') {
        return logError<std::unique_ptr<PrototypeAST>>("Expected '(' in prototype");
    }
    std::vector<std::pair<std::string, std::unique_ptr<TypeExprAST>>> arg_names;
    while(getNextToken() == tok_identifier) {
        auto name = lexer::state.identifier_str;
        getNextToken();
        if(cur_token != ':') {
            return logError<std::unique_ptr<PrototypeAST>>("Expected ':' after identifier");
        }
        getNextToken();
        auto type = parseTypeExpr();
        arg_names.emplace_back(name, std::move(type));
        if(cur_token != ',') {
            break;
        }
    }
    if(cur_token != ')') {
        return logError<std::unique_ptr<PrototypeAST>>("Expected ')' in prototype!");
    }
    getNextToken();
    std::unique_ptr<TypeExprAST> type = nullptr;
    if(cur_token == ':') {
        getNextToken();
        type = parseTypeExpr();
    }

    if(kind && arg_names.size() != kind) {
        return logError<std::unique_ptr<PrototypeAST>>("Invalid number of operants");
    }

    return std::make_unique<PrototypeAST>(fn_name, std::move(type), std::move(arg_names), kind != 0, binaryPrecedence);
}

std::unique_ptr<FunctionAST> parser::parseDefinition() {
    getNextToken();
    auto proto = parsePrototype();
    if(!proto) return nullptr;
    if(auto e = parseExpression()) {
        return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
    }
    return nullptr;
}

std::unique_ptr<PrototypeAST> parser::parseExtern() {
    getNextToken();
    return parsePrototype();
}

std::unique_ptr<FunctionAST> parser::parseTopLevelExpr() {
    if(auto e = parseExpression()) {
        auto proto = std::make_unique<PrototypeAST>("", nullptr, std::vector<std::pair<std::string, std::unique_ptr<TypeExprAST>>>());
        return std::make_unique<FunctionAST>(std::move(proto), std::move(e));
    }
    return nullptr;
}

void parser::mainLoop() {
    while(true) {
        switch(cur_token) {
            case tok_exit:
            case tok_eof:
                return;
            case tok_fun:
                handleDefinition();
                break;
            case tok_extern:
                handleExtern();
                break;
            default:
                handleTopLevelExpression();
                break;
        }
    }
}

void parser::handleDefinition() {
    if(auto fnAST = parseDefinition()) {
        if(auto *fnIR = fnAST->codegen()) {
            std::cout << "Read function definition:";
            auto stream = llvm::raw_os_ostream(std::cout);
            fnIR->print(stream);
            std::cout << '\n';
        }
    } else {
        getNextToken();
    }
}

void parser::handleExtern() {
    if(auto protoAST = parseExtern()) {
        if(auto* fnIR = protoAST->codegen()) {
            std::cout << "Read extern:";
            auto stream = llvm::raw_os_ostream(std::cout);
            fnIR->print(stream);
            std::cout << '\n';
        }
    } else {
        getNextToken();
    }
}

void parser::handleTopLevelExpression() {
    if(auto fnAST = parseTopLevelExpr()) {
        if(auto* fnIR = fnAST->codegen()) {
            std::cout << "Read top-level:";
            auto stream = llvm::raw_os_ostream(std::cout);
            fnIR->print(stream);
            std::cout << '\n';
        }
    } else {
        getNextToken();
    }
}

std::unique_ptr<ExprAST> parser::parseIfExpr() {
    getNextToken();
    auto Cond = parseExpression();
    if(!Cond) {
        return nullptr;
    }
    auto Then = parseExpression();
    if(!Then) {
        return nullptr;
    }
    std::unique_ptr<ExprAST> Else = nullptr;
    if(cur_token == Token::tok_else) {
        getNextToken();
        Else = parseExpression();
    }
    return std::make_unique<IfExprAST>(std::move(Cond), std::move(Then), std::move(Else));
}

std::unique_ptr<ExprAST> parser::parseForExpr() {
    getNextToken();
    if(cur_token != '(') {
        return logError<std::unique_ptr<ExprAST>>("expected '(' after for");
    }
    getNextToken();
    if(cur_token != tok_identifier) {
        return logError<std::unique_ptr<ExprAST>>("expected identifier after for");
    }
    std::string id_name = lexer::state.identifier_str;
    getNextToken();
    if(cur_token != '=') {
        return logError<std::unique_ptr<ExprAST>>("expected = after identifier");
    }
    getNextToken();
    auto Start = parseExpression();
    if(!Start) {
        return nullptr;
    }
    if(cur_token != ',') {
        return logError<std::unique_ptr<ExprAST>>("expected , after start");
    }

    getNextToken();

    auto End = parseExpression();
    if(!End) {
        return nullptr;
    }

    std::unique_ptr<ExprAST> Step;
    if(cur_token == ',') {
        getNextToken();
        Step = parseExpression();
        if(!Step) {
            return nullptr;
        }
    }
    if(cur_token != ')') {
        return logError<std::unique_ptr<ExprAST>>("expected ) at the end");
    }
    getNextToken();

    auto Body = parseExpression();
    if(!Body) {
        return nullptr;
    }

    return std::make_unique<ForExprAST>(id_name, std::move(Start), std::move(End), std::move(Step), std::move(Body));
}

std::unique_ptr<ExprAST> parser::parseCharacterExpr() {
    char prevChar = lexer::state.last_char;
    getNextToken();
    char res = ([=](){
        if(cur_token == Token::tok_identifier) {
            return lexer::state.identifier_str[0];
        }
        if(cur_token < 0) {
            return prevChar;
        }
        return (char)cur_token;
    })();
    if(cur_token == '\\') {
        getNextToken();
        switch(lexer::state.identifier_str[0]) {
            case 'n':
                res = '\n';
                break;
            case 'r':
                res = '\r';
                break;
        }
    }
    getNextToken();
    if(cur_token != '\'') {
        return logError<std::unique_ptr<ExprAST>>("Expected ' after character");
    }
    getNextToken();
    return std::make_unique<CharacterExprAST>(res);
}

std::unique_ptr<ExprAST> parser::parseUnary() {
    if(!isascii(cur_token) || cur_token == '(' || cur_token == ',' || cur_token == '\'' || cur_token == '{' || cur_token == '}' || cur_token == '[') {
        return parsePrimary();
    }
    int opc = cur_token;
    getNextToken();
    if(auto operand = parseUnary()) {
        return std::make_unique<UnaryExprAST>(opc, std::move(operand));
    }
    return nullptr;
}

std::unique_ptr<ExprAST> parser::parseBlockExpr() {
    std::vector<std::unique_ptr<ExprAST>> expressions;
    std::unique_ptr<ExprAST> retExpr = nullptr;
    getNextToken();
    bool ignore = false;
    while(cur_token != '}') {
        if(cur_token == Token::tok_ret) {
            retExpr = std::move(parseExpression());
            ignore = true;
        }
        if(auto expr = parseExpression()) {
            if(!ignore) {
                expressions.push_back(std::move(expr));
            }
        }
    }
    getNextToken();

    return std::make_unique<BlockExprAST>(std::move(expressions), std::move(retExpr));
}

std::unique_ptr<ExprAST> parser::parseReturnExpr() {
    getNextToken();
    return std::make_unique<ReturnExprAST>(std::move(parseExpression()));
}

std::unique_ptr<ExprAST> parser::parseMutExpr() {
    getNextToken();
    std::vector<std::tuple<std::string, std::unique_ptr<ExprAST>, std::unique_ptr<TypeExprAST>>>  var_names;
    if(cur_token != Token::tok_identifier) {
        return logError<std::unique_ptr<ExprAST>>("expected identifier after mut");
    }
    while(true) {
        auto name = lexer::state.identifier_str;
        getNextToken();
        std::unique_ptr<ExprAST> init;
        std::unique_ptr<TypeExprAST> type;
        if(cur_token == ':') {
            getNextToken();
            type = parseTypeExpr();
            if(!type) {
                return nullptr;
            }
        }
        if(cur_token == '=') {
            getNextToken();

            init = parseExpression();
            if(!init) {
                return nullptr;
            }
        }

        var_names.emplace_back(name, std::move(init), std::move(type));

        if(cur_token != ',') break;
        getNextToken();
        if(cur_token != tok_identifier) {
            return logError<std::unique_ptr<ExprAST>>("expected identifier list after mut");
        }
    }
    return std::make_unique<MutExprAST>(std::move(var_names));
}

std::unique_ptr<TypeExprAST> parser::parseTypeExpr() {
    if(cur_token != tok_identifier) {
        return nullptr;
    }
    auto typeName = lexer::state.identifier_str;
    auto isReference = false;
    auto isArray = false;
    getNextToken();
    if(cur_token == '&') {
        isReference = true;
        getNextToken();
    }
    std::vector<std::unique_ptr<ExprAST>> arraySize;
    if(cur_token == '[') {
        isArray = true;
        while(cur_token == '[' || cur_token == ',') {
            getNextToken();
            auto arrSizeExpr = parseExpression();
            if(!arrSizeExpr) {
                return logError<std::unique_ptr<TypeExprAST>>("expected array size");
            }
            arraySize.push_back(std::move(arrSizeExpr));

            if(cur_token != ']' && cur_token != ',') {
                return logError<std::unique_ptr<TypeExprAST>>("expected ]");
            }
        }
        getNextToken();
    }
    return std::make_unique<TypeExprAST>(typeName, isReference, isArray, std::move(arraySize));
}

std::unique_ptr<ExprAST> parser::parseArrayExpr() {
    return nullptr;
}
