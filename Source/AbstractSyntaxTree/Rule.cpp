#include <utility>

#include <utility>
#include "Rule.h"

ACC::Rule::Rule(const ACC::Production &p) : production(p){

}

ACC::Rule::Rule(const ACC::Production &p, const ACC::ruleExecuter_t &f) : production(p), func(f){

}

ACC::ASTNode *ACC::Rule::apply(std::vector<ACC::ParseNode *> children, ASTNode* carry, ACC::AbstractSyntaxTree& tree) const {
    return func(std::move(children), carry, tree);
}
