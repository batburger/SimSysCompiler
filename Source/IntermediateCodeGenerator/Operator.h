//
// Created by a_mod on 10.02.2019.
//

#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <Logger/Logger.h>

namespace ACC{
    using address = uint32_t;
    using temporary = uint32_t;

    enum class OperatorId{
        ADD,
        IADD,
        SUBTRACT,
        ISUBTRACT,
        COPY,
        ICOPY,
        PRINT
    };

    struct Operator{
        Operator(OperatorId id, address lhs, address rhs, temporary result) :  id(id), lhs(lhs), rhs(rhs), result(result) {};
        OperatorId id;
        address lhs;
        address rhs;
        temporary result;

        Operator* opLhs = nullptr;
        Operator* opRhs = nullptr;
        Operator* opResult = nullptr;

        void printDependency(std::string indent, bool isLast) const;
        std::string asString() const;
    };


}