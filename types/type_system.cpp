//
// Created by simi on 1/26/22.
//

#include "type_system.h"
#include "generator/generator.h"

llvm::Type *type_system::generate_type(const std::string &type) {
    if(type == "number") {
        return llvm::Type::getDoubleTy(*generator::theContext);
    }
    if(type == "char") {
        return llvm::Type::getInt8Ty(*generator::theContext);
    }
    if(type == "boolean") {
        return llvm::Type::getInt1Ty(*generator::theContext);
    }
    return llvm::Type::getDoubleTy(*generator::theContext);
}
