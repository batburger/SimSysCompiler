//
// Created by a_mod on 09.01.2019.
//

#ifndef SIMSYSCOMPILER_VARTOKEN_H
#define SIMSYSCOMPILER_VARTOKEN_H

#include <Lexical/IToken.h>

namespace ACC{
    struct VarToken : public IToken{
        VarToken() : IToken() {id = Symbol::VAR;};
        friend inline bool operator==(const VarToken&, const VarToken&){
            return true;
        }
    };
}

#endif //SIMSYSCOMPILER_VARTOKEN_H
