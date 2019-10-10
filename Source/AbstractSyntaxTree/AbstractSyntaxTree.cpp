//
// Created by a_mod on 23.01.2019.
//

#include "AbstractSyntaxTree.h"
#include <AbstractSyntaxTree/process.h>
#include <sstream>
#include <Logger/Logger.h>
#include <AbstractSyntaxTree/ASTNodes/TypeNode.h>
#include <Types/Type.h>
#include <Types/TypeTable.h>

ACC::AbstractSyntaxTree::AbstractSyntaxTree(ACC::ParseTree parseTree) {
    root = process(parseTree.getRoot(), *this, nullptr);
    refCount++;
}

void ACC::AbstractSyntaxTree::print() {
    std::stringstream ss;
    ss << "Generated Abstract Syntax Tree: (Node at:" << std::hex << this << " used as root)";
    Log::Logger::get()->createHeading(ss.str());
    root->_print("", false);
}

ACC::ASTNode *ACC::AbstractSyntaxTree::getRoot() const {
    return root;
}

ACC::AbstractSyntaxTree::~AbstractSyntaxTree() {
    refCount--;
    if (refCount != 0)
        return;

    delete root;
}

ACC::AbstractSyntaxTree::AbstractSyntaxTree(const ACC::AbstractSyntaxTree &other) : root(other.root),
                                                                                    refCount(other.refCount) {
    refCount++;
}

void ACC::AbstractSyntaxTree::doTypeResolution() {
    std::unordered_map<std::string, std::vector<TypeExport *>> dependencyMap;
    std::vector<TypeExport> knownSizes;

    for (auto &type : typeExports) {
        size_t total = 0;
        for (const auto& pair : type.fields) {
            TypeNode* field = pair.second;
            std::string const & name = field->getUnverifiedName();
            bool clean = true;
            if (name == "num") total += 8;
            else if (name == "char") total += 1;
            else if (name == "bool") total += 1;
            else if (name == "ptr") total += 8;
            else {
                if (dependencyMap.find(name) == dependencyMap.cend())
                    dependencyMap[name] = std::vector<TypeExport *>();

                dependencyMap[name].push_back(&type);
                clean = false;
            }
            if (clean)
                type.resolvedFields++;
        }
        type.size = total;
        if (type.resolvedFields == type.fields.size())
            knownSizes.push_back(type);
    }



    while (knownSizes.size() != typeExports.size()) {
        bool touched = false;
        for (size_t idx = 0; idx < knownSizes.size(); idx++) {

            for (size_t i = 0; i < dependencyMap[knownSizes[idx].name].size(); i++) {
                touched = true;
                TypeExport *dependent = dependencyMap[knownSizes[idx].name].at(i);
                dependent->size += knownSizes[idx].size;
                dependent->resolvedFields++;
                dependencyMap[knownSizes[idx].name].erase(dependencyMap[knownSizes[idx].name].begin() + i);

                if (dependent->resolvedFields == dependent->fields.size())
                    knownSizes.push_back(*dependent);

            }
        }
        if(!touched)
            throw std::runtime_error("yeet");
    }

    for (auto& typeExport : knownSizes){
        std::unordered_map<std::string, TypeField> fields;
        std::unordered_map<std::string, size_t> fieldMap;
        size_t totalSize = 0;

        for(auto const & field : typeExport.fields){
            auto typeName = field.second->getUnverifiedName();
            auto fieldName = field.first;

            fields[fieldName]  = TypeField(fieldName, typeName, false);
            fieldMap[fieldName] = totalSize;
            totalSize += typeExport.size;
        }

        Type type = Type(typeExport.name, totalSize, fields);
        type.fieldSizes = fieldMap;
        type.isComplex = true;
        TypeTable::get()->addType(typeExport.name, type);
    }


}
