#include "ReturnTokenGenerator.h"

ACC::ReturnTokenGenerator::ReturnTokenGenerator(ACC::ASTNode *node) : Expr(node) {

}

ACC::Structure ACC::ReturnTokenGenerator::generate(ACC::Code &code) {
    auto returnValue = node->children[0]->asExpr()->generate(code);
    auto& fn = code.getFnSymbol();

    fn.writeLine("add rsp, " + std::to_string(fn.curBpOffset - 1));
    if(returnValue.type == StructureType::elementary)
        fn.writeLine(returnValue.copyToRegister("rax"));
    else{} // TODO: Implement.

    fn.writeLine("pop rbp");
    fn.writeLine("ret");

    code.popFnFromStack();
    return {};
}