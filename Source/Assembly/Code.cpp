#include <utility>

//
// Created by a_mod on 10.02.2019.
//

#include <Logger/Logger.h>
#include "Code.h"
#include <General/utils.h>
#include <BuiltInFunctions/getFunctions.h>
#include <General/builtinTypes.h>
#include <Error/Errors.h>
#include <BuiltInFunctions/ByteWiseCopy.h>

ACC::Code::Code() {
    globalScope = std::make_shared<ScopedSymbolTable<std::shared_ptr<Structure>>>();
    curScope = globalScope.get();

    for (size_t i = 0; i < 14; i++) // TODO: This is a bit disgusting.
        freeRegisterTable[(Register) i] = true;

    // Builtin Types
    
    
    // Builtin Functions
    emplaceFnSymbol("char") = BuiltIns::getCharFn_num();
    emplaceFnSymbol("num") = BuiltIns::getNumFn_char();
    emplaceFnSymbol("?byteWiseCopy") = BuiltIns::byteWiseCopy::get();

    emplaceFnSymbol("_start");

};

std::shared_ptr<ACC::Structure> ACC::Code::getVarSymbol(std::string sym) {
    if (!curScope->isSymbol(sym))
        throw errors::UnknownVariable(nullptr, sym);
    else
        return curScope->getSymbol(sym);
}

std::shared_ptr<ACC::Structure> ACC::Code::emplaceVarSymbol(std::string sym, std::shared_ptr<Structure> struc) {
    curScope->symbolTable[sym] = std::move(struc);
    return curScope->symbolTable[sym];
}


std::vector<ACC::Fn> &ACC::Code::getFnOverloads(std::string sym) {
    if (fnTable.find(sym) == fnTable.end())
        throw errors::MissingOverload(nullptr, sym);
    else
        return fnTable.at(sym);
}

ACC::Fn& ACC::Code::emplaceFnSymbol(const std::string &sym) {
    if(fnTable.find(sym) == fnTable.cend()){
        // There are no overloads with symbol `sym`
        fnTable[sym] = std::vector<Fn> {Fn(sym)};

        fnStack.push({sym, 0});
        return fnTable.at(sym)[0];
    }

    // There are overloads with symbol `sym`
    fnTable[sym].push_back(Fn(sym));
    fnStack.push({sym, fnTable.at(sym).size() - 1});
    return fnTable.at(sym)[fnTable.at(sym).size() - 1];


}

ACC::Fn& ACC::Code::getFnSymbol() {
    return fnTable.at(fnStack.peek().first).at(fnStack.peek().second);
}


std::string ACC::Code::combineOutput() {
    std::string out = "section .data\n" + dataSection;
    out += "\nsection .text\n";

    for (auto const &overloads : fnTable){
        for(auto const & function : overloads.second){
            out += "global " + function.mangledName() + "\n";
        }
    }

    for (auto const &overloads : fnTable){
        for(auto const & function : overloads.second){
            out += function.mangledName() + ":\n" + function.generate();
        }
    }

    return out;
}

void ACC::Code::writeLineToData(std::string const &str) {
    dataSection += str + "\n";
}

void ACC::Code::reserveRegister(ACC::Register reg) {
    // reserve
    if (freeRegisterTable[reg] == false)
        throw std::runtime_error("Trying to reserve register which isn't free.");
    freeRegisterTable[reg] = false;
}

bool ACC::Code::isRegisterFree(ACC::Register reg) {
    bool out = freeRegisterTable.at(reg);
    return out;
}

ACC::Register ACC::Code::getFreeRegister() {
    for (size_t i = 13; i >= 0; i--) { // TODO: This is a bit disgusting.
        if (freeRegisterTable[(Register) i]) {
            reserveRegister((Register) i);
            return (Register) i;
        }
    }
    throw std::runtime_error("No free register available!");
}

void ACC::Code::freeRegister(ACC::Register reg) {
    // Free
    freeRegisterTable[reg] = true;
}

void ACC::Code::freeRegister(std::vector<ACC::Register> reg) {
    for (const auto &r : reg)
        freeRegister(r);
}

void ACC::Code::popFnFromStack() {
    fnStack.pop();
}

void ACC::Code::pushScope() {
    curScope = new ScopedSymbolTable<std::shared_ptr<Structure>>(curScope);
}

void ACC::Code::popScope() {
    auto old = curScope;
    curScope->prev->next = nullptr;
    curScope = curScope->prev;
    delete old;
}

std::string ACC::Code::getUUID() {
    return numberToLetterSequence(uuidCounter++);
}

bool ACC::Code::hasOverload(const std::string &sym, const Type& returnType, const std::vector<Type>& argsType) {
    if(fnTable.find(sym) == fnTable.end())
        return false;

    auto overloads = fnTable[sym];

    for(Fn const & overload : overloads) {
        if (overload.returnType != returnType)
            continue;

        if(overload.argsType == argsType)
            return true;
    }

    return false;
}

std::string ACC::registerToString(size_t size, ACC::Register reg) {
    switch (reg) {
        case Register::rA: {
            if (size == 1) return "al";
            if (size == 2) return "ax";
            if (size == 4) return "eax";
            if (size == 8) return "rax";
        }
            break;
        case Register::rB: {
            if (size == 1) return "bl";
            if (size == 2) return "bx";
            if (size == 4) return "ebx";
            if (size == 8) return "rbx";
        }
            break;
        case Register::rC: {
            if (size == 1) return "cl";
            if (size == 2) return "cx";
            if (size == 4) return "ecx";
            if (size == 8) return "rcx";
        }
            break;
        case Register::rD: {
            if (size == 1) return "dl";
            if (size == 2) return "dx";
            if (size == 4) return "edx";
            if (size == 8) return "rdx";
        }
            break;
        case Register::rSI: {
            if (size == 1) return "sil";
            if (size == 2) return "si";
            if (size == 4) return "esi";
            if (size == 8) return "rsi";
        }
            break;
        case Register::rDI: {
            if (size == 1) return "dil";
            if (size == 2) return "di";
            if (size == 4) return "edi";
            if (size == 8) return "rdi";
        }
            break;
        case Register::r8: {
            if (size == 1) return "r8b";
            if (size == 2) return "r8w";
            if (size == 4) return "r8d";
            if (size == 8) return "r8";
        }
            break;
        case Register::r9: {
            if (size == 1) return "r9b";
            if (size == 2) return "r9w";
            if (size == 4) return "r9d";
            if (size == 8) return "r9";
        }
            break;
        case Register::r10: {
            if (size == 1) return "r10b";
            if (size == 2) return "r10w";
            if (size == 4) return "r10d";
            if (size == 8) return "r10";
        }
            break;
        case Register::r11: {
            if (size == 1) return "r11b";
            if (size == 2) return "r11w";
            if (size == 4) return "r11d";
            if (size == 8) return "r11";
        }
            break;
        case Register::r12: {
            if (size == 1) return "r12b";
            if (size == 2) return "r12w";
            if (size == 4) return "r12d";
            if (size == 8) return "r12";
        }
            break;
        case Register::r13: {
            if (size == 1) return "r13b";
            if (size == 2) return "r13w";
            if (size == 4) return "r13d";
            if (size == 8) return "r13";
        }
            break;
        case Register::r14: {
            if (size == 1) return "r14b";
            if (size == 2) return "r14w";
            if (size == 4) return "r14d";
            if (size == 8) return "r14";
        }
            break;
        case Register::r15: {
            if (size == 1) return "r15b";
            if (size == 2) return "r15w";
            if (size == 4) return "r15d";
            if (size == 8) return "r15";
        }
            break;
    }
    return "";
}
