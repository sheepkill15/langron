//
// Created by simi on 1/22/22.
//

#include <iostream>
#include "lexer.h"
#include "token.h"

LexerState lexer::state = LexerState();

int lexer::gettok() {
    while(std::isspace(state.last_char)) {
        if(!std::cin.get(state.last_char)) {
            return Token::tok_eof;
        }
    }
    if(std::isalpha(state.last_char)) {
        state.identifier_str = state.last_char;
        std::cin.get(state.last_char);
        while(std::isalnum(state.last_char)) {
            state.identifier_str.append(1, state.last_char);
            std::cin.get(state.last_char);
        }
        if(state.identifier_str == "fun") {
            return Token::tok_fun;
        }
        if(state.identifier_str == "extern") {
            return Token::tok_extern;
        }
        if(state.identifier_str == "if") {
            return Token::tok_if;
        }
        if(state.identifier_str == "else") {
            return Token::tok_else;
        }
        if(state.identifier_str == "for") {
            return Token::tok_for;
        }
        if(state.identifier_str == "binary") {
            return Token::tok_binary;
        }
        if(state.identifier_str == "unary") {
            return Token::tok_unary;
        }
        if(state.identifier_str == "ret") {
            return Token::tok_ret;
        }
        if(state.identifier_str == "exit") {
            return Token::tok_exit;
        }
        return Token::tok_identifier;
    }
    if(std::isdigit(state.last_char) || state.last_char == '.') {
        std::string num_str;
        do {
            num_str += state.last_char;
            std::cin.get(state.last_char);
        } while(std::isdigit(state.last_char) || state.last_char == '.');
        state.number_value = strtod(num_str.c_str(), nullptr);
        return Token::tok_number;
    }

    if(state.last_char == '#') {
        do {
            std::cin.get(state.last_char);
        } while(state.last_char != EOF && state.last_char != '\n' && state.last_char != '\r');
        if(state.last_char != EOF) {
            return gettok();
        }
    }
    if(state.last_char == EOF || std::cin.eof()) {
        return Token::tok_eof;
    }
    int this_char = state.last_char;
    std::cin.get(state.last_char);
    return this_char;
}
