//
// Created by a_mod on 06.01.2019.
//

#include "LexicalAnalysis.h"
#include <fstream>
#include <iostream>
#include <Lexical/Tokens/EOSToken.h>
#include <errors.h>
#include <Logger/Logger.h>
#include <SymbolTable.h>

#include <Lexical/Tokens/LiteralToken.h>
#include <Lexical/Tokens/IdToken.h>
#include <Lexical/Tokens/VarToken.h>
#include <Lexical/Tokens/BracketToken.h>
#include <Lexical/Tokens/MathOperatorToken.h>
#include <Lexical/Tokens/PrintToken.h>
#include <Lexical/Tokens/AssignToken.h>
#include <Lexical/Tokens/DeclToken.h>
#include <Lexical/Tokens/ExitToken.h>

ACC::LexicalAnalysis::LexicalAnalysis(std::string path){
    refCount++;
    std::ifstream fs;
    fs.open(path);
    this->document = std::string((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

    LOG.createHeading("Original Input being Lexically Analysed:");
    LOG() << this->document << std::endl;

    preProcessDocument();
    process();
}

ACC::LexicalAnalysis::LexicalAnalysis(const ACC::LexicalAnalysis &other)
        : tokens(other.tokens), document(other.document), refCount(other.refCount),
        processed(other.processed)
{
    refCount++;
}


void ACC::LexicalAnalysis::process() {
    if(processed)
        throw repeated_step_error_t("The file has already been analysed!");
    LOG.createHeading("Starting Lexical Analysis...");
    processed = true;

    std::string buffer;
    for(auto itr = document.begin(); itr != document.end(); ) {
        if(*itr == ' '){
            itr++;
            continue;
        }
        if(isNumber(*itr)){ // is c a number
            for(;isNumber(*itr); itr++)
                buffer += *itr;
            LOG() << Log::Colour::Magenta << "Detected Literal: ";
            LOG() << buffer << std::endl;
            tokens.push_back(new LiteralToken(buffer));
            buffer.clear();
        }else{
            bool isInTable = false;
            for(;itr != document.end(); itr++){
                if(inTable(buffer)) {
                    isInTable = true;
                    break;
                }
                if(*itr == ' ')
                    break;
                buffer += *itr;
            }
            if(!isInTable){ //not in table
                LOG() << Log::Colour::Magenta << "Not in table: ";
                LOG() << buffer << std::endl;
                SymbolTable::get()->emplace(buffer, Symbol::DECL);
                tokens.push_back(new DeclToken(buffer));
                buffer.clear();
            }
            else{ // in table
                LOG() << Log::Colour::Magenta << "In Table: ";
                LOG() << buffer << std::endl;
                switch(SymbolTable::get()->at(buffer)){
                    case Symbol::empty:break;
                    case Symbol::ID:break;
                    case Symbol::none_terminals_start:break;
                    case Symbol::LITERAL:break;
                    case Symbol::expr:break;
                    case Symbol::declaration:break;
                    case Symbol::assignment:break;
                    case Symbol::keyword:break;
                    case Symbol::stmt:break;
                    case Symbol::start:break;

                    case Symbol::VAR:
                        tokens.push_back(new VarToken());
                        break;
                    case Symbol::BRACKET:
                        tokens.push_back(new BracketToken(buffer == "(" ? (BracketKind::OPEN) : (BracketKind::CLOSED)));
                        break;
                    case Symbol::MATH_OPERATOR:
                        tokens.push_back(new MathOperatorToken(buffer == "+" ? (MathOperators::PLUS) : (MathOperators::MINUS)));
                        break;
                    case Symbol::PRINT:
                        tokens.push_back(new PrintToken());
                        break;
                    case Symbol::EOS:
                        tokens.push_back(new EOSToken());
                        break;
                    case Symbol::ASSIGN:
                        tokens.push_back(new AssignToken());
                        break;
                    case Symbol::DECL:
                        tokens.push_back(new IdToken(buffer));
                        break;
                    case Symbol::EXIT:
                        tokens.push_back(new ExitToken());
                        break;
                }
                buffer.clear();
            }
        }
    }
}

ACC::LexicalAnalysis::~LexicalAnalysis() {
    refCount--;
    if(refCount != 0)
        return;

    SymbolTable::get()->del();
    for(const auto& it : tokens)
        delete it;
}


void ACC::LexicalAnalysis::preProcessDocument() {
    for(long i = document.size() - 1; i >= 0; i--){
        char cur = document.at(i);
        if(cur == '\n' || cur == '\r')
            document.erase(i, 1);
        else if (i - 1 >= 0 && cur == ' ' && document[i - 1] == ' ')
            document.erase(i, 1);
    }
    document += ' ';
}

void ACC::LexicalAnalysis::printToken() {
    LOG.createHeading("Tokens generated by the Lexical Analysis:");
    for(auto const& token : tokens){
        LOG() << token->getIdentifier() << std::endl;
    }
}

const std::vector<ACC::IToken *, std::allocator<ACC::IToken *>>::iterator ACC::LexicalAnalysis::begin() {
    return tokens.begin();
}

const std::vector<ACC::IToken *, std::allocator<ACC::IToken *>>::iterator ACC::LexicalAnalysis::end() {
    return tokens.end();
}

const std::vector<ACC::IToken *> &ACC::LexicalAnalysis::data() {
    return tokens;
}

bool ACC::LexicalAnalysis::inTable(std::string idf) {
    return SymbolTable::get()->find(idf) != SymbolTable::get()->end();
}

bool ACC::LexicalAnalysis::isNumber(char c) {
    return (int)c >= 48 && (int)c <=57;
}