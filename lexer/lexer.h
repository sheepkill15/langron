//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_LEXER_H
#define LANGRON_LEXER_H

#include <string>

struct LexerState {
    char last_char = ' ';
    std::string identifier_str;
    double number_value;
};

class lexer {
public:
    lexer() = delete;
    static int gettok();

    static LexerState state;
};


#endif //LANGRON_LEXER_H
