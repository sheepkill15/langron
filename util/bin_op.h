//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_BIN_OP_H
#define LANGRON_BIN_OP_H

#include <map>
#include <cctype>

static std::map<std::string, int> binOpPrecedence = {
        {
            "<", 10
        },
        {
            "+", 20
        },
        {
            "-", 20
        },
        {
            "*", 40
        },
        {
            "=", 2
        },
        {
            "==", 9
        }
};

static int getTokPrecedence(const std::string& token) {
    if(!binOpPrecedence.contains(token)) {
        return -1;
    }
    int tokPrec = binOpPrecedence.at(token);
    if(tokPrec <= 0) return -1;
    return tokPrec;
}

#endif //LANGRON_BIN_OP_H
