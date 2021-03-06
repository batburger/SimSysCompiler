#include <utility>

#include <utility>

#include <utility>

#include <utility>
#include "ASTNode.h"
#include <AbstractSyntaxTree/process.h>
#include <Logger/Logger.h>

#include <AbstractSyntaxTree/ASTNodes/AddNode.h>
#include <AbstractSyntaxTree/ASTNodes/SubtractNode.h>
#include <AbstractSyntaxTree/ASTNodes/LiteralNode.h>
#include <AbstractSyntaxTree/ASTNodes/SeqNode.h>
#include <AbstractSyntaxTree/ASTNodes/AssignNode.h>
#include <AbstractSyntaxTree/ASTNodes/IdNode.h>
#include <AbstractSyntaxTree/ASTNodes/WhileNode.h>
#include <AbstractSyntaxTree/ASTNodes/ForNode.h>
#include <AbstractSyntaxTree/ASTNodes/SyscallNode.h>
#include <AbstractSyntaxTree/ASTNodes/ExitNode.h>
#include <AbstractSyntaxTree/ASTNodes/DivisionNode.h>
#include <AbstractSyntaxTree/ASTNodes/MultiplicationNode.h>
#include <AbstractSyntaxTree/ASTNodes/FunctionNode.h>
#include <AbstractSyntaxTree/ASTNodes/ReturnNode.h>
#include <AbstractSyntaxTree/ASTNodes/ReassignNode.h>
#include <AbstractSyntaxTree/ASTNodes/CallNode.h>
#include <AbstractSyntaxTree/ASTNodes/ComparisionNode.h>
#include <AbstractSyntaxTree/ASTNodes/NotNode.h>
#include <AbstractSyntaxTree/ASTNodes/IfConstructNode.h>
#include <General/GeneralDataStore.h>
#include <General/builtinTypes.h>

ACC::ASTNode::ASTNode(AstOperator op, std::vector<ACC::ASTNode*> children) {
    this->op = op;
    this->children = std::move(children);
}


ACC::ASTNode::ASTNode() {
    this->op = AstOperator::__NONE;
}
/*
ACC::ASTNode::ASTNode(AstOperator op, ACC::GeneralDataStore literal, Type type) {
    this->op = op;
    this->data = std::move(literal);
    this->type = std::move(type);
}


ACC::ASTNode::ASTNode(AstOperator op, std::string str) {
    if(op == AstOperator::LITERAL)
        throw std::runtime_error("Initialise literals with a general store");
    this->op = op;
    this->type = BuiltIns::__none;
    this->data.storeT(std::move(str));
}


ACC::ASTNode::ASTNode(ACC::AstOperator op, ACC::Type store) {
    if(op == AstOperator::LITERAL)
        throw std::runtime_error("Initialise literals with a general store");
}

ACC::ASTNode::ASTNode(AstOperator op) {
    if(op != AstOperator::__NONE)
        throw std::runtime_error("Single argument constructor for `ASTNode` can only be used for `AstOperator::__NONE`");
    this->op = op;
}
 */

void ACC::ASTNode::_print(std::string indent, bool isLast) const {
    std::string representation = createRepresentation();
    auto colour = op == AstOperator::LITERAL ? (Log::Colour::Magenta) : (Log::Colour::Blue);

    LOG() << indent;
    if(isLast){
        LOG() << "\\-";
        indent += "     ";
    }else{
        LOG() << "|-";
        indent += "|    ";
    }
    LOG() << colour << "[" << representation << "]" << std::endl;

    for (int i = 0; i < children.size(); i++) {
        children[i]->_print(indent, i == children.size() - 1);
    }
}

ACC::ASTNode::~ASTNode() {
    for(const auto& child : children){
        delete child;
    }
}

std::shared_ptr<ACC::Structure> ACC::ASTNode::generate(ACC::Code &) {
    throw std::runtime_error("Can't generate on operator `"+ createRepresentation() +"`");
}

std::string ACC::ASTNode::createRepresentation() const {
    switch (op){
        case AstOperator::ADD:
            return "+";
        case AstOperator::SUBTRACT:
            return "-";
        case AstOperator::LITERAL:{
            return "Literal";
        }
        case AstOperator::SEQ:
            return "SEQ";
        case AstOperator::ASSIGN:
            return "assign";
        case AstOperator::ID:
            return "id";
        case AstOperator::SYSCALL:
            return "syscall";
        case AstOperator::EXIT:
            return "exit";
        case AstOperator::MULTIPLICATION:
            return "*";
        case AstOperator::DIVISION:
            return "/";
        case AstOperator::FUNCTION:
            return "function";
        case AstOperator::CALL:
            return "call";
        case AstOperator::RETURN:
            return "return";
        case AstOperator::__NONE:
            return "__none";
        case AstOperator::TYPE_DEF:
            return "type def";
        case AstOperator::__CONTAINER:
            return "__container";
        case AstOperator::REASSIGN:
            return "reassign";
        case AstOperator::IF_CONSTRUCT:
            return "if construct";
        case AstOperator::EQUAL:
            return "equal";
        case AstOperator::NOT_EQUAL:
            return "not equal";
        case AstOperator::LESS:
            return "less";
        case AstOperator::GREATER:
            return "greater";
        case AstOperator::LESS_EQUAL:
            return "less equal";
        case AstOperator::GREATER_EQUAL:
            return "greater equal";
        case AstOperator::NOT:
            return "not";
        case AstOperator::IF:
            return "if";
        case AstOperator::ELIF:
            return "elif";
        case AstOperator::ELSE:
            return "else";
        case AstOperator::WHILE:
            return "while";
        case AstOperator::FOR:
            return "for";
        case AstOperator::PTR_ASSIGN:
            return "ptr_assign";
        case AstOperator::SALLOC:
            return "salloc";
        case AstOperator::DEREFERENCE:
            return "dereference";
        case AstOperator::MEMBER_ACCESS:
            return "member access";
        case AstOperator::MEMBER_CALL:
            return "member call";
        case AstOperator::TYPE_DECL_BODY:
            return "type decl body";
        case AstOperator::TYPE_DECL:
            return "type decl";
        case AstOperator::INITIALIZER_LIST:
            return "initializer list";
        case AstOperator::AND:
            return "and";
        case AstOperator::OR:
            return "or";
    }
    throw std::runtime_error("Unknown Symbol!");

}

