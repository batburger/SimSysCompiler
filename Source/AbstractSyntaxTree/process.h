//
// Created by a_mod on 23.01.2019.
//

#ifndef SIMSYSCOMPILER_PROCESS_H
#define SIMSYSCOMPILER_PROCESS_H

#include <AbstractSyntaxTree/ASTNode.h>
#include <Parser/ParseNode.h>
#include <astRules.h>
#include <AbstractSyntaxTree/AbstractSyntaxTree.h>

namespace ACC{
    /*! Given a `ParseNode`, this will return the `Rule` which will generate the abstract syntax tree for this parse node.
     * @param node the node of the Parse Tree for which the Rule should be found.
     * @return A `Rule` object that can be used to generate the Abstract Syntax Tree Node for the given ParseNode.*/
    Rule getRule(const ParseNode* node);

    /*! Returns true if the `Rule` object given through rule is the rule used to generate the Abstract Syntax Tree Nodes
     * for the `ParseNode` given through node.
     * @param node The ParseNode object pointer, which should be checked.
     * @param rule The Rule object to check if it fits the ParseNode.
     * @return Returns true if the rule is applicable for the AbstractSyntaxTree Node generation considering the ParseNodes given as node.*/
    bool matches(const ParseNode* node, const Rule& rule);

    /*! Process the parse tree from `node` down, when turning a Parse Tree to a Abstract Syntax Tree.
     * @param node The `ParseNode` object pointer to process.
     * @param carry A `ASTNode` pointer that may be given when process is called from a rule generating the Abstract Syntax Tree.
     * @return Returns the `ASTNode` pointer to the ASTNode object created by processing the parse tree from `node` down.*/
    ACC::ASTNode *process(const ACC::ParseNode *node, AbstractSyntaxTree &tree, ASTNode *carry = nullptr);
}


#endif //SIMSYSCOMPILER_PROCESS_H
