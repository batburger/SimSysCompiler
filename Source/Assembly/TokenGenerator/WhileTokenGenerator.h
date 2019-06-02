#include <utility>

//
// Created by a_mod on 09.01.2019.
//
#pragma once

#include <Assembly/Expr.h>
#include <Assembly/Code.h>


namespace ACC{
    struct WhileTokenGenerator : public Expr{
        Structure generate(ACC::Code &code) override;
        explicit WhileTokenGenerator(ASTNode* node);

    };
}
