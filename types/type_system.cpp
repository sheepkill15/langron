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

llvm::Value *type_system::generate_cast(llvm::Value *srcValue, llvm::Type *destType, const std::string& variableName) {
    auto srcType = srcValue->getType();
    if(srcType->isPointerTy() && destType->isPointerTy()) {
        return srcValue;
    }
    if(srcType->isPointerTy()) {
        return generate_cast(generator::builder->CreateLoad(srcType->getContainedType(0), srcValue, variableName), destType, variableName);
    }
    if(srcType->isIntegerTy()) {
        if(destType->isFloatingPointTy()) {
            return generator::builder->CreateSIToFP(srcValue, destType, variableName);
        }
        return srcValue;
    }
    if(srcType->isFloatingPointTy()) {
        if(destType->isIntegerTy()) {
            return generator::builder->CreateFPToSI(srcValue, destType, variableName);
        }
    }
    return srcValue;
}

llvm::Value *type_system::depointify(llvm::Value *srcValue, const std::string &variableName) {
    if(srcValue->getType()->isPointerTy()) {
        srcValue = generator::builder->CreateLoad(srcValue->getType()->getContainedType(0), srcValue);
        return depointify(srcValue, variableName);
    }
    return srcValue;
}
