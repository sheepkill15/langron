#include <iostream>
#include <fstream>
#include "types/type_system.h"
#include "parser/parser.h"
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <cstdlib>
#include <filesystem>

std::ifstream fin;

int main(int argc, char* argv[]) {
    std::cin >> std::noskipws;
    auto buf = std::cin.rdbuf();
    if(argc > 1) {
        const auto filename = argv[1];
        fin = std::ifstream(filename);
        std::cin.rdbuf(fin.rdbuf());
        std::cout << "Reading from file: " << filename << '\n';
    }
    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, error);
    if(!Target) {
        std::cerr << error;
        return 1;
    }

    auto CPU = "x86-64";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    parser::getNextToken();
    generator::initialize();
    parser::mainLoop();
    auto layout = TargetMachine->createDataLayout();
    generator::theModule->setDataLayout(layout);
    generator::theModule->setTargetTriple(TargetTriple);

    std::filesystem::path file;

    if(argc > 1) {
        file = argv[1];
        file.replace_filename("output.o");
    } else {
        file = "output.o";
    }
    std::error_code EC;
    llvm::raw_fd_ostream dest(file.c_str(), EC, llvm::sys::fs::OF_None);

    if(EC) {
        std::cerr << "Could not open file: " << EC.message();
        return 1;
    }

    llvm::legacy::PassManager pass;
    auto FileType = llvm::CGFT_ObjectFile;
    if(TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        std::cerr << "Target machine can't emit a file of this type";
        return 1;
    }
    pass.run(*generator::theModule);
    dest.flush();
    auto mainFile = file;
    mainFile.replace_filename("main");
    mainFile.replace_extension("");
    system(("g++ " + file.string() + " ../runtime/* -no-pie" + " -o " + mainFile.string()).c_str());
    std::cin.rdbuf(buf);
    system(mainFile.c_str());

    return 0;
}
