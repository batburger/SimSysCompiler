//
// Created by a_mod on 25.01.2019.
//

#ifndef SIMSYSCOMPILER_RULE_H
#define SIMSYSCOMPILER_RULE_H

#include <Parser/Production.h>
#include <functional>
#include <AbstractSyntaxTree/ASTNode.h>
#include <AbstractSyntaxTree/AbstractSyntaxTree.h>

namespace ACC{
    using ruleExecuter_t = std::function<ACC::ASTNode*(std::vector<ACC::ParseNode*>, ACC::ASTNode*, ACC::AbstractSyntaxTree& tree)>;

    /*! A Rule for turning a ParseNode to a Abstract Syntax Tree, it fits only one `Production`. */
    struct Rule {
        Rule() = delete;
        Rule(const Production& p);
        Rule(const Production& p, const ruleExecuter_t& f);

        const Production production;

    /*! Applies the rule to the parse tree from `node` down, when turning a Parse Tree to a Abstract Syntax Tree.
     * @param node The `ParseNode` object pointer to apply the rule to.
     * @param carry A `ASTNode` pointer that may be given when apply is called from a rule generating the Abstract Syntax Tree.
     * @return Returns the `ASTNode` pointer to the ASTNode object created by applying the rule to the parse tree from `node` down.*/
        ASTNode* apply(std::vector<ParseNode*> children, ASTNode* carry, ACC::AbstractSyntaxTree& tree) const;

    private:
        const ruleExecuter_t func;
    };
}

#endif //SIMSYSCOMPILER_RULE_H
