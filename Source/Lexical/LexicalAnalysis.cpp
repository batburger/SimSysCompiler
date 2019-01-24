//
// Created by a_mod on 06.01.2019.
//

#include "LexicalAnalysis.h"
#include <fstream>
#include <Lexical/IContext.h>
#include <Lexical/Contexts/GlobalContext.h>
#include <Lexical/Contexts/AssignmentContext.h>
#include <iostream>
#include <Lexical/Tokens/EOSToken.h>
#include <errors.h>

ACC::LexicalAnalysis::LexicalAnalysis(std::string path){
    refCount++;
    std::ifstream fs;
    fs.open(path);
    this->document = std::string((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

    contextStack.push(new GlobalContext());
    preProcessDocument();
    process();
}

ACC::LexicalAnalysis::LexicalAnalysis(const ACC::LexicalAnalysis &other)
        : contextStack(other.contextStack), tokens(other.tokens), document(other.document), refCount(other.refCount),
        processed(other.processed)
{
    refCount++;
}


void ACC::LexicalAnalysis::process() {
    if(processed)
        throw repeated_step_error_t("The file has already been analysed!");
    processed = true;
    size_t range = 1;
    IContext::match expr;

    for(auto itr = document.begin(); (itr+range-1) != document.end();){
        auto context = contextStack.peek();
        std::string debug(itr, itr + range);
        if(context->escapeSequence().matches(itr, range)){
            contextStack.pop();
            tokens.push_back(new EOSToken());
            itr += range;
            range = 1;
            continue;
        }

        if(!matches(context, itr, range, &expr)){
            range++;
            continue;
        }

        while(expr.first.matches(itr, range)){
            range++;
        }
        range--;
        if(expr.second.id == InstructionId::NEW_TOKEN){
            auto func = *static_cast<Instruction::token_func*>(expr.second.func);
            auto token = func(document, itr, itr+range);
            tokens.push_back(token);
        }
        else if(expr.second.id == InstructionId::CHANGE_CONTEXT) {
            contextStack.push(static_cast<Instruction::context_func *>(expr.second.func)->operator()());
        }
        itr += range;
        range = 1;
    }

}

ACC::LexicalAnalysis::~LexicalAnalysis() {
    refCount--;
    if(refCount != 0)
        return;

    for(const auto& it : contextStack)
        delete it;
    contextStack.destroy();

    for(const auto& it : tokens)
        delete it;
}

bool ACC::LexicalAnalysis::matches(ACC::IContext *context, const std::string::iterator &itr, size_t range, ACC::IContext::match *legalExpr) {
    int amount = 0;
    for(auto expr : context->getLegals()){
        if(expr.first.matches(itr, range)){
            amount++;
            *legalExpr = expr;
        }
    }
    return !(amount == 0 || amount > 1);
}

void ACC::LexicalAnalysis::preProcessDocument() {
    for(int i = document.size() - 1; i >= 0; i--){
        char cur = document.at(i);
        if(cur == '\n' || cur == '\r')
            document.erase(i, 1);
        else if (i - 1 >= 0 && cur == ' ' && document[i - 1] == ' ')
            document.erase(i, 1);
    }
}

void ACC::LexicalAnalysis::printToken() {
    for(auto const& token : tokens){
        std::cout << (int)token->id << " ";
    }
    std::cout << std::endl;
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
