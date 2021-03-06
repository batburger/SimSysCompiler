#include <utility>

//
// Created by a_mod on 06.01.2019.
//
#pragma once


#include <AbstractSyntaxTree/ASTNode.h>
#include <Assembly/Code.h>


namespace ACC{
    struct IdNode : public ASTNode{
        std::shared_ptr<Structure> generate(ACC::Code &code) override;
        IdNode(AstOperator op, std::string str);
        std::string sym;

        std::string createRepresentation() const override;

    };
}
