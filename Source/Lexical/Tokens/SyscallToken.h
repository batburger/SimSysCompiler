#include <utility>

//
// Created by a_mod on 09.01.2019.
//

#ifndef SIMSYSCOMPILER_PRINTTOKEN_H
#define SIMSYSCOMPILER_PRINTTOKEN_H

#include <Lexical/IToken.h>

namespace ACC{
    struct SyscallToken : public IToken{

        explicit SyscallToken(size_t lineNum) : IToken(Symbol::SYSCALL, lineNum) {}

        std::string getIdentifier() override{
            return "syscall";
        }

        [[nodiscard]] std::string getIdForErrReporting() const override{
            return "syscall";
        };

        friend inline bool operator==(SyscallToken const & lhs, SyscallToken const & rhs){
            return true;
        }
    };
}


#endif //SIMSYSCOMPILER_PRINTTOKEN_H
