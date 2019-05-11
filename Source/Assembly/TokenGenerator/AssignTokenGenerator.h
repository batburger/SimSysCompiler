#include <utility>

//
// Created by a_mod on 09.01.2019.
//

#pragma once

#include <Assembly/Expr.h>
#include <Assembly/Code.h>

namespace ACC{
    struct AssignTokenGenerator : public Expr{
        Structure generate(ACC::Code &code) override;

        explicit AssignTokenGenerator(ASTNode* node);

    private:
        ACC::Structure handleElementary(ACC::Code &code, std::string id, TypeId type, Structure expr);

    };
}

