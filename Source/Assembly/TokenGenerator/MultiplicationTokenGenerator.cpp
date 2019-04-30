#include "MultiplicationTokenGenerator.h"

ACC::MultiplicationTokenGenerator::MultiplicationTokenGenerator(ACC::ASTNode *node) : Expr(node) {

}

ACC::Structure ACC::MultiplicationTokenGenerator::generate(ACC::Code &code) {
    auto lhs = node->children[0]->asExpr()->generate(code);
    auto rhs = node->children[1]->asExpr()->generate(code);
    auto& fn = code.getFnSymbol();

    auto lhsRegister = code.getFreeRegister();
    auto rhsRegister = code.getFreeRegister();

    if(lhs.type == StructureType::elementary){
        fn.writeLine(lhs.copyToRegister(registerToString(8, lhsRegister)));
    }

    if(rhs.type == StructureType::elementary){
        fn.writeLine(rhs.copyToRegister(registerToString(8, rhsRegister)));
    }

    fn.writeLine("imul "+ registerToString(8, lhsRegister) +", "+ registerToString(8, rhsRegister));

    auto return_struct = Structure(StructureType::elementary);

    return_struct.copyToRegister = [=](std::string reg){
        if(reg != "rax")
            return "mov " + reg + ", " + registerToString(8, lhsRegister);
        return std::string();
    };

    return_struct.copyToStack = [=](){
        return "mov qword [rsp], " + registerToString(8, lhsRegister);
    };

    code.freeRegister(rhsRegister);
    code.freeRegister(lhs.registerUsed);
    code.freeRegister(rhs.registerUsed);
    return_struct.registerUsed = {lhsRegister};

    return return_struct;
}