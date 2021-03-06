//
// Created by a_mod on 09.01.2019.
//

#pragma once

#include <Lexical/IToken.h>

namespace ACC{
    struct ElseToken : public IToken{
        explicit ElseToken(size_t lineNum) : IToken(Symbol::ELSE, lineNum) {};

        std::string getIdentifier() override{
            return "else";
        }

        [[nodiscard]] std::string getIdForErrReporting() const override{
            return "else";
        };

        friend inline bool operator==(const ElseToken&, const ElseToken&){
            return true;
        }
    };
}