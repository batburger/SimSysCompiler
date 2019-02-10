#include "SeqTokenGenerator.h"

ACC::temporary ACC::SeqTokenGenerator::generate(ACC::Code &code) {
    if(node->children.size() == 2)
        node->children[1]->asExpr()->generate(code);
    node->children[0]->asExpr()->generate(code);
    return 0;
}

ACC::SeqTokenGenerator::SeqTokenGenerator(ACC::ASTNode *node) : Expr(node) {

}
