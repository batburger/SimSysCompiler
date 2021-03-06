//
// Created by a_mod on 20.08.2019.
//
#pragma once

#include <AbstractSyntaxTree/ASTNode.h>
#include <Types/UnverifiedType.h>
#include <Types/Type.h>

namespace ACC {
    class TypeDeclBodyNode : public ASTNode {
    public:
        TypeDeclBodyNode(AstOperator op, std::vector<ASTNode*> children, std::string sym);

        std::shared_ptr<Structure> generate(ACC::Code &code) override;

        [[nodiscard]] std::string createRepresentation() const override;

        Type getType();
        std::string getName();


    private:
        std::string sym;

    };
}