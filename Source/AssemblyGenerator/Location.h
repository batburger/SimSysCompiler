//
// Created by a_mod on 02.03.2019.
//



#ifndef SIMSYSCOMPILER_SNIPPET_H
#define SIMSYSCOMPILER_SNIPPET_H

#include <string>
#include <variant>
#include <utility>
#include <stdexcept>

namespace ACC {
    enum class AccessMethod{
        REGISTER,
        STACK_TOP,
        STACK_OFFSET,
        SBP_OFFSET,
        HEAP,
        CONSTANT,
        NONE
    };

    enum class Register{
        rax,
        rbx,
        rcx,
        rdx,
        rbp,
        rsi,
        rdi,
        rsp,
        r8,
        r9,
        r10,
        r11,
        r12,
        r13,
        r14,
        r15,
        NONE
    };

    using ptr_t = uint64_t;
    using offset_t = signed long;
    struct Location{
        Location() = default;

        explicit Location(AccessMethod aAccessMethod) : accessMethod(aAccessMethod) {}

        AccessMethod accessMethod = AccessMethod::NONE;
        Register regInfo = Register::NONE;
        offset_t offsetInfo = 0;
        ptr_t heapInfo = 0;
        std::string constant;
    };
}

#endif //SIMSYSCOMPILER_SNIPPET_H