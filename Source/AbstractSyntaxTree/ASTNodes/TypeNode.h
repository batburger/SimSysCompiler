//
// Created by a_mod on 20.08.2019.
//
#pragma once

#include <AbstractSyntaxTree/ASTNode.h>
#include <Types/UnverifiedType.h>

namespace ACC {
    class TypeNode : public ASTNode {
    public:
        TypeNode(AstOperator op, Type t);
        TypeNode(AstOperator op, UnverifiedType t);


        std::shared_ptr<Structure> generate(ACC::Code &code) override;

        std::string createRepresentation() const override;

        Type getType();

        std::string getUnverifiedName();

    private:
        UnverifiedType unverifiedType;

        Type type = Type("", 0);
    };
}