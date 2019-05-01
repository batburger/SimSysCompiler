//
// Created by a_mod on 10.02.2019.
//

#ifndef SIMSYSCOMPILER_CODE_H
#define SIMSYSCOMPILER_CODE_H

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <Stack.h>

namespace ACC{

    enum class StructureType{
        elementary,
        complex,
        NA
    };

    enum class Register;
    class Code;

    struct Structure{
        Structure() = default;
        explicit Structure(StructureType type) : type(type) {};

        std::function<std::string (Code&)> copyToStack;
        std::function<std::string (Code&)> copyAddressToStack;
        std::function<std::string (std::string, Code&)> copyToRegister;
        std::function<std::string (std::string, Code&)> copyAddressToRegister;
        std::function<std::string (int32_t , Code&)> copyToBpOffset;
        std::function<std::string (int32_t, Code&)> copyAddressToBpOffset;
        std::function<std::string (Code&)> rawValue;

        StructureType type = StructureType::NA;
        size_t size = 0;
        std::vector<Register> registerUsed;
    };

    struct Fn{
        std::string code;
        std::string symbol;
        uint32_t curBpOffset = 17;

        void writeLine(std::string const & line);
        std::string generate();
    };

    enum class Register{
        rA,
        rB,
        rC,
        rD,
        rSI,
        rDI,
        r8,
        r9,
        r10,
        r11,
        r12,
        r13,
        r14,
        r15,
    };

    std::string registerToString(size_t size, Register reg);

    class Code {
    private:
        std::unordered_map<std::string, Structure> varTable;
        std::unordered_map<std::string, Fn> fnTable;
        std::unordered_map<Register, bool> freeRegisterTable;

        Stack<std::string> fnStack;
        int ref = 1;
        std::string dataSection;

    public:
        Code();

        void reserveRegister(Register reg);
        void freeRegister(Register reg);
        void freeRegister(std::vector<Register> regs);
        bool isRegisterFree(Register reg);
        void popFnFromStack();
        Register getFreeRegister();

        Structure& getVarSymbol(std::string sym);
        Structure& emplaceVarSymbol(std::string sym, const Structure &struc);

        Fn& getFnSymbol(std::string sym);
        Fn& getFnSymbol();
        Fn& emplaceFnSymbol(std::string sym);

        void writeLineToData(std::string const & str);
        std::string combineOutput();
    };
}

#endif //SIMSYSCOMPILER_CODE_H
