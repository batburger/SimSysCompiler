#include <utility>

//
// Created by a_mod on 09.01.2019.
//

#pragma once

#include <IntermediateCodeGenerator/Expr.h>
#include <IntermediateCodeGenerator/Code.h>

namespace ACC{
    struct AssignTokenGenerator : public Expr{
        Dependency generate(ACC::Code &code) override;

        explicit AssignTokenGenerator(ASTNode* node);

    };
}
