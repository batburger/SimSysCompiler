//
// Created by a_mod on 06.01.2019.
//

#pragma once

#include <Stack.h>
#include <Lexical/IContext.h>

namespace ACC {
    class LexicalAnalysis{
    private:
        Stack<IContext*> contextStack;
        std::vector<IToken*> tokens;
        std::string document;
        int refCount = 0;
        bool processed = false;

        void preProcessDocument();
        bool matches(IContext* context, const std::string::iterator& itr, size_t range, IContext::match* legalExpr);
    public:
        explicit LexicalAnalysis(std::string path);
        LexicalAnalysis(const LexicalAnalysis& other);
        ~LexicalAnalysis();
        void process();
        void printToken();

        const std::vector<IToken*>::iterator begin();
        const std::vector<IToken*>::iterator end();
        std::vector<IToken*> const & data();
    };
}