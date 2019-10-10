 //
// Created by a_mod on 23.01.2019.
//

#include "process.h"

ACC::ASTNode *ACC::process(const ACC::ParseNode *node, AbstractSyntaxTree &tree, ASTNode *carry) {
    Rule r = getRule(node);
    auto n = r.apply(node->children, carry, tree);

    if(node->children[0]->token && n) {
        n->lineContent = node->children[0]->token->lineContent;
        n->lineNum = node->children[0]->token->lineNum;
    }

    if(node->children[0]->token && carry) {
        carry->lineContent = node->children[0]->token->lineContent;
        carry->lineNum = node->children[0]->token->lineNum;
    }

    return n;
}

 ACC::Rule ACC::getRule(const ACC::ParseNode* node) {
    for(const auto& rule : data::getRules()){
        if(!matches(node, rule))
            continue;
        else
            return rule;
    }

    std::string production = data::symbolToString(node->symbol) + " :=";
    for (auto child : node->children){
        production += " " + data::symbolToString(child->symbol);
    }

    throw std::runtime_error("Production is not contained in abstract syntax tree rules. \n" + production);
 }

 bool ACC::matches(const ACC::ParseNode *node, const Rule &rule) {
    if(node->symbol != rule.production.head)
        return false;

    if(rule.production.body.size() != node->children.size())
        return false;

    size_t idx = 0;
    for(const auto& child : node->children){
        if(child->symbol != rule.production.body[idx])
            return false;
        idx++;
    }
     return true;
 }
