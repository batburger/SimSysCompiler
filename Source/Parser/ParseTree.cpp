#include <utility>

//
// Created by a_mod on 10.01.2019.
//

#include <exception>
#include <Lexical/Tokens/VarToken.h>
#include <Lexical/Tokens/ClosedBracketToken.h>
#include <iostream>
#include "ParseTree.h"
#include "Production.h"
#include <Logger/Logger.h>
#include <Lexical/Tokens/IndentToken.h>
#include <Logger/LogableProduction.h>
#include <Error/SyntaxError.h>
#include <Lexical/Tokens/ComparisionToken.h>

#define START_PRODUCTION() \
    logable.echoProduction(); \
    { \
    int atleast = 0; \
    bool b = [&]() {

#define END_PRODUCTION() \
    return true; \
    }(); \
    if (b){ \
         pos--;\
        return node; \
    }\
    else { \
        if(atleast > 0){ \
            lastProduction = logable; \
            lastLogablePos = atleast; \
            unexpected = document.at(pos); \
        } \
        killChildren(node); \
        pos = oldPos;\
    }\
    }

#define NONE_TERMINAL(name) \
    if (other = name(pos), other == nullptr) { \
        logable.red(); \
        return false; \
    } \
    atleast++; \
    pos++; \
    node->children.push_back(other); \
    logable.green();

#define TERMINAL(name) \
    if (other = match(pos, Symbol::name), other == nullptr) { \
        logable.red(); \
        return false; \
    } \
    atleast++; \
    pos++; \
    node->children.push_back(other); \
    logable.green();

#define EXITING_OPTIONAL_NONE_TERMINAL(name) \
    if(other = name(pos), other == nullptr) { \
        logable.blue(); \
        return true; \
    } \
    else{ \
        logable.green(); \
        atleast++; \
        pos++; \
        node->children.push_back(other); \
    }

#define EXITING_OPTIONAL_TERMINAL(name) \
    if(other = match(pos, Symbol::name), other == nullptr) {\
        logable.blue(); \
        return true; \
    }\
    else { \
        logable.green();\
        atleast++;\
        pos++; \
        node->children.push_back(other); \
    }


#define OPTIONAL_NONE_TERMINAL(name) \
    if (other = name(pos), other != nullptr){\
        atleast++; \
        node->children.push_back(other);\
        logable.green(); \
        pos++;\
    } \
    else {\
        logable.blue();\
    }\

#define OPTIONAL_TERMINAL(name) \
    if (other = match(pos, Symbol::name), other != nullptr){\
        atleast++; \
        node->children.push_back(other);\
        logable.green(); \
        pos++;\
    } \
    else {\
        logable.blue();\
    }\


#define EXITING_OPTIONAL_TERMINAL_WITH_PROPERTY(name, type, property, value) \
    if (other = match(pos, Symbol::name), other != nullptr && dynamic_cast<type *>(document[pos]) && dynamic_cast<type *>(document[pos])->property == value) {\
        logable.green();\
        atleast++;\
        pos++;\
        node->children.push_back(other);\
    } else {\
        logable.blue();\
        return true;\
    }

#define TERMINAL_WITH_PROPERTY(name, type, property, value) \
    if (other = match(pos, Symbol::name), other != nullptr && dynamic_cast<type *>(document[pos]) && dynamic_cast<type *>(document[pos])->property == value) {\
        logable.green();\
        atleast++;\
        pos++;\
        node->children.push_back(other);\
    } else {\
        logable.red();\
        return false;\
    }

ACC::ParseTree::ParseTree(const ACC::LexicalAnalysis &in) {
    generate(in);
}

ACC::ParseTree::ParseTree(const ACC::ParseTree &other) : refCount(other.refCount), root(other.root),
                                                         generated(other.generated) {
    refCount++;
}

void ACC::ParseTree::killChildren(ACC::ParseNode *node) {
    for (auto const &child : node->children)
        delete child;
    node->children.clear();
}

const ACC::ParseNode *ACC::ParseTree::getRoot() {
    return root;
}

void ACC::ParseTree::generate(ACC::LexicalAnalysis in) {
    if (generated)
        throw std::runtime_error("The parse tree has already been generated.");
    generated = true;
    LOG.createHeading("Generating Parse Tree...");
    //  root = process(const_cast<LexicalAnalysis &>(in).data(), Symbol::start);
    document = in.data();
    size_t pos = 0;
    root = start(pos);


    if(pos + 1 != in.size()) {
        throw errors::SyntaxError(unexpected->lineNum, unexpected->lineContent, unexpected, lastProduction.at(lastLogablePos));
    }
}

ACC::ParseTree::~ParseTree() {
    refCount--;
    if (refCount > 0)
        return;

    delete root;
    root = nullptr;
}

ACC::ParseNode *ACC::ParseTree::match(size_t &pos, ACC::Symbol what){
    if (pos >= document.size())
        return nullptr;

    if (document[pos]->id == what)
        return new ParseNode(document[pos]->id, document[pos]);
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::start(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [start]...\n";
    ParseNode *node = new ParseNode;
    node->symbol = Symbol::start;

    size_t oldPos = pos;

    ParseNode *other;

    logable.loadProduction(Symbol::start, {Symbol::assignment, Symbol::EOS, Symbol::start});
    START_PRODUCTION()

        NONE_TERMINAL(assignment)
        TERMINAL(EOS)
        OPTIONAL_NONE_TERMINAL(start);

    END_PRODUCTION()


    logable.loadProduction(Symbol::start, {Symbol::if_construct, Symbol::start});
    START_PRODUCTION()
            NONE_TERMINAL(ifConstruct)
            OPTIONAL_NONE_TERMINAL(start);

    END_PRODUCTION()

    logable.loadProduction(Symbol::start, {Symbol::function, Symbol::EXTENT, Symbol::start});
    START_PRODUCTION()

            NONE_TERMINAL(function)
            TERMINAL(EXTENT)
            OPTIONAL_NONE_TERMINAL(start);

    END_PRODUCTION()

    logable.loadProduction(Symbol::start, {Symbol::keyword, Symbol::EOS, Symbol::start});
    START_PRODUCTION()

            NONE_TERMINAL(keyword)
            TERMINAL(EOS)
            OPTIONAL_NONE_TERMINAL(start);

    END_PRODUCTION()


    logable.loadProduction(Symbol::start, {Symbol::while_construct, Symbol::start});
    START_PRODUCTION()

            NONE_TERMINAL(whileConstruct)
            OPTIONAL_NONE_TERMINAL(start);

    END_PRODUCTION()

    logable.loadProduction(Symbol::start, {Symbol::for_construct, Symbol::start});
    START_PRODUCTION()

            NONE_TERMINAL(forConstruct)
            OPTIONAL_NONE_TERMINAL(start);

    END_PRODUCTION()

    logable.loadProduction(Symbol::start, {Symbol::type_decl, Symbol::start});
    START_PRODUCTION()
            NONE_TERMINAL(typeDecl)
            OPTIONAL_NONE_TERMINAL(start);

    END_PRODUCTION()

    logable.loadProduction(Symbol::start, {Symbol::trait, Symbol::EXTENT, Symbol::start});
    START_PRODUCTION()
            NONE_TERMINAL(trait)
            TERMINAL(EXTENT)
            OPTIONAL_NONE_TERMINAL(start);

    END_PRODUCTION()

    logable.loadProduction(Symbol::start, {Symbol::expr, Symbol::EOS, Symbol::start});
    START_PRODUCTION()
            NONE_TERMINAL(expr)
            TERMINAL(EOS)
            OPTIONAL_NONE_TERMINAL(start);

    END_PRODUCTION()

    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::assignment(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [assignment]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::assignment;
    ParseNode *other;

    size_t oldPos = pos;

    logable.loadProduction(Symbol::assignment, {Symbol::VAR, Symbol::TEXT, Symbol::COLON, Symbol::type, Symbol::ASSIGN, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(VAR)
            TERMINAL(TEXT)
            TERMINAL(COLON)
            NONE_TERMINAL(type)
            TERMINAL(ASSIGN)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::assignment, {Symbol::STAR, Symbol::expr, Symbol::ASSIGN, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(STAR)
            NONE_TERMINAL(expr)
            TERMINAL(ASSIGN)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::assignment, {Symbol::expr, Symbol::ASSIGN, Symbol::expr});
    START_PRODUCTION()
            NONE_TERMINAL(expr)
            TERMINAL(ASSIGN)
            NONE_TERMINAL(expr)
    END_PRODUCTION()




    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::function(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [function]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::function;

    size_t oldPos = pos;

    ParseNode *other;

    logable.loadProduction(Symbol::function, {Symbol::FUNCTION, Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::paramsDecl,
                                              Symbol::CLOSED_BRACKET, Symbol::ARROW,  Symbol::type, Symbol::COLON, Symbol::INDENT, Symbol::start});
    START_PRODUCTION()
            TERMINAL(FUNCTION)
            TERMINAL(TEXT)
            TERMINAL(OPEN_BRACKET)
            OPTIONAL_NONE_TERMINAL(paramDecl)
            TERMINAL(CLOSED_BRACKET)
            TERMINAL(ARROW)
            NONE_TERMINAL(type)
            TERMINAL(COLON)
            TERMINAL(INDENT)
            NONE_TERMINAL(start)
    END_PRODUCTION()

    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::keyword(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [keyword]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::keyword;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::keyword, {Symbol::EXIT, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(EXIT)
            NONE_TERMINAL(expr)
    END_PRODUCTION()


    logable.loadProduction(Symbol::keyword, {Symbol::RETURN, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(RETURN)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::keyword, {Symbol::SALLOC, Symbol::expr, Symbol::COMMA, Symbol::TEXT});
    START_PRODUCTION()
            TERMINAL(SALLOC)
            NONE_TERMINAL(expr)
            TERMINAL(COMMA)
            TERMINAL(TEXT)
    END_PRODUCTION()

    logable.loadProduction(Symbol::keyword, {Symbol::SYSCALL, Symbol::expr, Symbol::COMMA});
    START_PRODUCTION()
            std::vector<Symbol> vec = {Symbol::SYSCALL, Symbol::expr, Symbol::COMMA};
            TERMINAL(SYSCALL)
            NONE_TERMINAL(expr)
            OPTIONAL_TERMINAL(COMMA)

            for(size_t i = 0; i < 6; i++){
                vec.push_back(Symbol::expr);
                vec.push_back(Symbol::COMMA);
                logable.changeProduction(Symbol::keyword, vec);
                OPTIONAL_NONE_TERMINAL(expr)
                OPTIONAL_TERMINAL(COMMA)
            }

    END_PRODUCTION()

    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::expr(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [expr]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::expr;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::expr, {Symbol::QUOTE, Symbol::TEXT, Symbol::QUOTE});
    START_PRODUCTION()
            TERMINAL(QUOTE)
            TERMINAL(TEXT)
            TERMINAL(QUOTE)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::DOUBLE_QUOTE, Symbol::TEXT, Symbol::DOUBLE_QUOTE});
    START_PRODUCTION()
            TERMINAL(DOUBLE_QUOTE)
            TERMINAL(TEXT)
            TERMINAL(DOUBLE_QUOTE)
    END_PRODUCTION()


    logable.loadProduction(Symbol::expr, {Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::CLOSED_BRACKET, Symbol::expr});
    START_PRODUCTION()
        TERMINAL(TEXT)
        TERMINAL(OPEN_BRACKET)
        TERMINAL(CLOSED_BRACKET)
        OPTIONAL_NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::paramsList, Symbol::CLOSED_BRACKET, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(TEXT)
            TERMINAL(OPEN_BRACKET)
            NONE_TERMINAL(paramList)
            TERMINAL(CLOSED_BRACKET)
            OPTIONAL_NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::OPEN_BRACKET, Symbol::expr, Symbol::CLOSED_BRACKET, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(OPEN_BRACKET)
            NONE_TERMINAL(expr)
            TERMINAL(CLOSED_BRACKET)
            OPTIONAL_NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::TRUE, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(TRUE)
            OPTIONAL_NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::FALSE, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(FALSE)
            OPTIONAL_NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::OR, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(OR)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::AND, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(AND)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::PLUS, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(PLUS)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::MINUS, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(MINUS)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::SLASH, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(SLASH)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::STAR, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(STAR)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::CMP, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(CMP)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::NOT, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(NOT)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::MODULO, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(MODULO)
            NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::DOT, Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::paramsList, Symbol::CLOSED_BRACKET, Symbol::expr });
    START_PRODUCTION()
            TERMINAL(DOT)
            TERMINAL(TEXT)
            TERMINAL(OPEN_BRACKET)
            OPTIONAL_NONE_TERMINAL(paramList)
            TERMINAL(CLOSED_BRACKET)
            OPTIONAL_NONE_TERMINAL(expr)
    END_PRODUCTION()

    logable.loadProduction(Symbol::expr, {Symbol::DOT, Symbol::TEXT, Symbol::expr });
    START_PRODUCTION()
            TERMINAL(DOT)
            TERMINAL(TEXT)
            OPTIONAL_NONE_TERMINAL(expr)
    END_PRODUCTION()



    logable.loadProduction(Symbol::expr, {Symbol::TEXT, Symbol::expr});
    START_PRODUCTION()
            TERMINAL(TEXT)
            OPTIONAL_NONE_TERMINAL(expr)
    END_PRODUCTION()


    logable.loadProduction(Symbol::expr, {Symbol::OPEN_CURLY, Symbol::initializer_list, Symbol::CLOSED_CURLY});
    START_PRODUCTION()
            TERMINAL(OPEN_CURLY)
            NONE_TERMINAL(initializerList)
            TERMINAL(CLOSED_CURLY)
    END_PRODUCTION()


    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::paramDecl(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [paramsDecl]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::paramsDecl;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::paramsDecl, {Symbol::TEXT, Symbol::COLON, Symbol::type, Symbol::COMMA, Symbol::paramsDecl});
    START_PRODUCTION()
            TERMINAL(TEXT)
            TERMINAL(COLON)
            NONE_TERMINAL(type)
            EXITING_OPTIONAL_TERMINAL(COMMA)
            NONE_TERMINAL(paramDecl)
    END_PRODUCTION()


    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::paramList(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [paramsList]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::paramsList;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::paramsList, {Symbol::expr, Symbol::COMMA, Symbol::paramsList});
    START_PRODUCTION()
            NONE_TERMINAL(expr)
            EXITING_OPTIONAL_TERMINAL(COMMA)
            NONE_TERMINAL(paramList)
    END_PRODUCTION()



    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::ifConstruct(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [if_construct]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::if_construct;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::if_construct, {Symbol::IF, Symbol::expr, Symbol::COLON, Symbol::INDENT, Symbol::start, Symbol::EXTENT,
                                                  Symbol::elseIf_construct, Symbol::else_construct});
    START_PRODUCTION()
            TERMINAL(IF)
            NONE_TERMINAL(expr)
            TERMINAL(COLON)
            TERMINAL(INDENT)
            NONE_TERMINAL(start)
            TERMINAL(EXTENT)
            OPTIONAL_NONE_TERMINAL(elifConstruct)
            OPTIONAL_NONE_TERMINAL(elseConstruct)
    END_PRODUCTION()

    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::elifConstruct(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [elif_construct]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::elseIf_construct;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::elseIf_construct, {Symbol::ELIF, Symbol::expr, Symbol::COLON,Symbol::INDENT,Symbol::start,
                                                      Symbol::EXTENT,Symbol::elseIf_construct});
    START_PRODUCTION()
            TERMINAL(ELIF)
            NONE_TERMINAL(expr)
            TERMINAL(COLON)
            TERMINAL(INDENT)
            NONE_TERMINAL(start)
            TERMINAL(EXTENT)
            OPTIONAL_NONE_TERMINAL(elifConstruct)
    END_PRODUCTION()


    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::elseConstruct(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [else_construct]...\n";
    ParseNode *node = new ParseNode;
    node->symbol = Symbol::else_construct;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::else_construct, {Symbol::ELSE, Symbol::COLON, Symbol::INDENT, Symbol::start, Symbol::EXTENT});
    START_PRODUCTION()
            TERMINAL(ELSE)
            TERMINAL(COLON)
            TERMINAL(INDENT)
            NONE_TERMINAL(start)
            TERMINAL(EXTENT)
    END_PRODUCTION()

    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;

}

ACC::ParseNode *ACC::ParseTree::whileConstruct(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [while_construct]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::while_construct;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::while_construct, {Symbol::WHILE, Symbol::expr, Symbol::COLON, Symbol::INDENT,
                                                     Symbol::start, Symbol::EXTENT});
    START_PRODUCTION()
            TERMINAL(WHILE)
            NONE_TERMINAL(expr)
            TERMINAL(COLON)
            TERMINAL(INDENT)
            NONE_TERMINAL(start)
            TERMINAL(EXTENT)
    END_PRODUCTION()

    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::forConstruct(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [for_construct]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::for_construct;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::for_construct, {Symbol::FOR, Symbol::expr, Symbol::ARROW, Symbol::expr, Symbol::COLON,
                                                   Symbol::INDENT, Symbol::start, Symbol::EXTENT});
    START_PRODUCTION()
            TERMINAL(FOR)
            NONE_TERMINAL(expr)
            TERMINAL(ARROW)
            NONE_TERMINAL(expr)
            TERMINAL(COLON)
            TERMINAL(INDENT)
            NONE_TERMINAL(start)
            TERMINAL(EXTENT)
    END_PRODUCTION()

    // for<TYPE> TEXT = expr ->

    logable.loadProduction(Symbol::for_construct, {Symbol::FOR, Symbol::CMP, Symbol::type, Symbol::CMP,
                                                   Symbol::TEXT, Symbol::ASSIGN, Symbol::expr, Symbol::ARROW, Symbol::expr, Symbol::COLON,
                                                   Symbol::INDENT, Symbol::start, Symbol::EXTENT});
    START_PRODUCTION()
            TERMINAL(FOR)
            TERMINAL_WITH_PROPERTY(CMP, ComparisionToken, kind, ComparisionTokenKind::Less)
            NONE_TERMINAL(type)
            TERMINAL_WITH_PROPERTY(CMP, ComparisionToken, kind, ComparisionTokenKind::Greater)
            TERMINAL(TEXT)
            TERMINAL(ASSIGN)
            NONE_TERMINAL(expr)
            TERMINAL(ARROW)
            NONE_TERMINAL(expr)
            TERMINAL(COLON)
            TERMINAL(INDENT)
            NONE_TERMINAL(start)
            TERMINAL(EXTENT)
    END_PRODUCTION()


    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::type(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [type]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::type;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::type, {Symbol::TEXT, Symbol::CMP, Symbol::TEXT, Symbol::CMP});
    START_PRODUCTION()
            TERMINAL(TEXT)
            EXITING_OPTIONAL_TERMINAL_WITH_PROPERTY(CMP, ComparisionToken, kind, ComparisionTokenKind::Less)
            TERMINAL(TEXT)
            TERMINAL_WITH_PROPERTY(CMP, ComparisionToken, kind, ComparisionTokenKind::Greater)

    END_PRODUCTION()

    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::typeDecl(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [type_decl]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::type_decl;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::type_decl, {Symbol::TYPE, Symbol::TEXT, Symbol::COLON, Symbol::INDENT, Symbol::type_decl_body, Symbol::EXTENT});
    START_PRODUCTION()
            TERMINAL(TYPE)
            TERMINAL(TEXT)
            TERMINAL(COLON)
            TERMINAL(INDENT)
            NONE_TERMINAL(typeDeclBody)
            TERMINAL(EXTENT)
    END_PRODUCTION()
    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::typeDeclBody(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [type_decl_body]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::type_decl_body;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::type_decl_body, {Symbol::TEXT, Symbol::COLON, Symbol::type, Symbol::EOS, Symbol::type_decl_body});
    START_PRODUCTION()

    TERMINAL(TEXT)
            TERMINAL(COLON)
            NONE_TERMINAL(type)
            TERMINAL(EOS)
            EXITING_OPTIONAL_NONE_TERMINAL(typeDeclBody)

    END_PRODUCTION()

    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::trait(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [trait]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::trait;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::function, {Symbol::TRAIT, Symbol::CMP, Symbol::TEXT ,Symbol::CMP, Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::paramsDecl,
                                              Symbol::CLOSED_BRACKET, Symbol::ARROW,  Symbol::type, Symbol::COLON, Symbol::INDENT, Symbol::start});
    START_PRODUCTION()
            TERMINAL(TRAIT)
            TERMINAL_WITH_PROPERTY(CMP, ComparisionToken, kind, ComparisionTokenKind::Less)
            TERMINAL(TEXT)
            TERMINAL_WITH_PROPERTY(CMP, ComparisionToken, kind, ComparisionTokenKind::Greater)
            TERMINAL(TEXT)
            TERMINAL(OPEN_BRACKET)
            OPTIONAL_NONE_TERMINAL(paramDecl)
            TERMINAL(CLOSED_BRACKET)
            TERMINAL(ARROW)
            NONE_TERMINAL(type)
            TERMINAL(COLON)
            TERMINAL(INDENT)
            NONE_TERMINAL(start)
    END_PRODUCTION()

    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}

ACC::ParseNode *ACC::ParseTree::initializerList(size_t &pos) {
    ACC::LogableProduction logable;
    LOG() << "\n";
    LOG() << Log::Colour::Magenta << "Entering [initializer_list]...\n";

    ParseNode *node = new ParseNode;
    node->symbol = Symbol::initializer_list;

    size_t oldPos = pos;
    ParseNode *other;

    logable.loadProduction(Symbol::initializer_list, {Symbol::expr, Symbol::COMMA, Symbol::initializer_list});
    START_PRODUCTION()
            NONE_TERMINAL(expr)
            EXITING_OPTIONAL_TERMINAL(COMMA)
            NONE_TERMINAL(initializerList)
    END_PRODUCTION()

    LOG() << Log::Colour::Magenta << "..done\n";

    pos = oldPos;
    delete node;
    return nullptr;
}


