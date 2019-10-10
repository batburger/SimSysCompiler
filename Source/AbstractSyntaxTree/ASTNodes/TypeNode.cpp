//
// Created by a_mod on 20.08.2019.
//

#include "TypeNode.h"
#include <Types/TypeTable.h>

#include <utility>
#include <Error/Errors.h>

ACC::TypeNode::TypeNode(ACC::AstOperator op, ACC::Type t) : ASTNode(op, {}), type(std::move(t)){

}

std::shared_ptr<ACC::Structure> ACC::TypeNode::generate(ACC::Code &code) {
    throw std::runtime_error("Can't generate type def node.");
}

ACC::TypeNode::TypeNode(ACC::AstOperator op, ACC::UnverifiedType t) : ASTNode(op, {}), unverifiedType(std::move(t)) {

}

ACC::Type ACC::TypeNode::getType() {
    if(type != Type("", 0))
        return type;

    if(!TypeTable::get()->isType(unverifiedType.id))
        throw errors::UnknownType(this, unverifiedType.id);

    auto out = TypeTable::get()->getType(unverifiedType.id);
    out.isPtr = unverifiedType.isPtr;
    if(unverifiedType.isPtr)
        out.size = 8;
    return out;
}

std::string ACC::TypeNode::createRepresentation() const {
    return "Type: " + unverifiedType.id + (unverifiedType.isPtr ? ("*"):(""));
}

std::string ACC::TypeNode::getUnverifiedName() {
    return unverifiedType.id;
}
