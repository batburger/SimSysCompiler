#include "IfConstructTokenGenerator.h"

ACC::IfConstructTokenGenerator::IfConstructTokenGenerator(ASTNode *node) : Expr(node) {

}

ACC::Structure ACC::IfConstructTokenGenerator::generate(ACC::Code &code) {
    auto &fn = code.getFnSymbol();
    auto final = code.getUUID();
    std::string next;
    if(node->children.size() == 1)
        next = final;
    else
        next = code.getUUID();

    code.pushScope();
    auto expr = node->children[0]->children[0]->asExpr()->generate(code);

    Register reg;
    if(!expr.registerUsed.empty())
        reg = expr.registerUsed[0];
    else
        reg = code.getFreeRegister();

    fn.writeLine(expr.copyToRegister(registerToString(1, reg), code));
    fn.writeLine("cmp " + registerToString(1, reg) + ", 1");
    fn.writeLine("jnz ." + next);

    code.freeRegister(expr.registerUsed);
    code.freeRegister(reg);

    node->children[0]->children[1]->asExpr()->generate(code);
    if(!node->children.empty()){
        fn.writeLine("jmp ." + final);
        for(size_t i = 1; i < node->children.size(); i++){
            if(node->children[i]->op == AstOperator::ELIF)
                handleElif(fn, code, i, next, final);
            else
                handleElse(fn, code, i, next);
        }
    }

    fn.writeLine("." + final + ":");


    code.popScope();
    return {};
}

void ACC::IfConstructTokenGenerator::handleElif(ACC::Fn &fn, ACC::Code &code, size_t idx, std::string &next,
                                                std::string const &final) {
    fn.writeLine("." + next + ":");

    if(node->children.size() - idx - 1 == 0)
        next = final;
    else
        next = code.getUUID();

    auto condition = node->children[idx]->children[0]->asExpr()->generate(code);

    Register reg;
    if(!condition.registerUsed.empty())
        reg = condition.registerUsed[0];
    else
        reg = code.getFreeRegister();

    fn.writeLine(condition.copyToRegister(registerToString(1, reg), code));
    fn.writeLine("cmp " + registerToString(1, reg) + ", 1");
    fn.writeLine("jnz ." + next);

    code.freeRegister(condition.registerUsed);
    code.freeRegister(reg);
    node->children[idx]->children[1]->asExpr()->generate(code);
    if(node->children.size() - idx - 1 != 0)
        fn.writeLine("jmp ." + final);
}

void ACC::IfConstructTokenGenerator::handleElse(ACC::Fn &fn, ACC::Code &code, size_t idx, std::string &next) {
    fn.writeLine("." + next + ":");
    node->children[idx]->children[0]->asExpr()->generate(code);
}