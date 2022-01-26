//
// Created by simi on 1/26/22.
//

#ifndef LANGRON_TYPE_SYSTEM_H
#define LANGRON_TYPE_SYSTEM_H


#include <llvm/IR/Type.h>

class type_system {
public:
    type_system() = delete;

    static llvm::Type* generate_type(const std::string& type);
};


#endif //LANGRON_TYPE_SYSTEM_H
