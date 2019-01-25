//
// Created by a_mod on 25.01.2019.
//

#ifndef SIMSYSCOMPILER_PRODUCTION_H
#define SIMSYSCOMPILER_PRODUCTION_H

#include <vector>

namespace ACC {
    enum class Symbol : int{
        empty = -1,
        VAR = 0,
        ID,
        BRACKET,
        MATH_OPERATOR,
        PRINT,
        LITERAL,
        EOS,

        none_terminals_start,

        expr,
        declaration,
        assignment,
        key,
        stmt,
        start
    };

    using productionBody_t = std::vector<Symbol>;

    struct Production {
        Production() = default;
        explicit Production(Symbol head);
        Production(Symbol head, std::vector<Symbol> body);

        Symbol head = Symbol::empty;
        std::vector<Symbol> body;
    };
}

#endif //SIMSYSCOMPILER_PRODUCTION_H