//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_TOKEN_H
#define LANGRON_TOKEN_H

enum Token {
    tok_eof = -1,

    // commands
    tok_fun = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,
    tok_exit = -6,
    tok_if = -7,
    tok_else = -8,
    tok_for = -9,
    tok_binary = -10,
    tok_unary = -11,
    tok_ret = -12
};

#endif //LANGRON_TOKEN_H
