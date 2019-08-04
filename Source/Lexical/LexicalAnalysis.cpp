//
// Created by a_mod on 06.01.2019.
//

#include <Lexical/Tokens/WhileToken.h>
#include <Lexical/Tokens/TypeToken.h>
#include "LexicalAnalysis.h"
#include <fstream>
#include <iostream>
#include <Lexical/Tokens/EOSToken.h>
#include <Logger/Logger.h>

#include <Lexical/Tokens/LiteralToken.h>
#include <Lexical/Tokens/IdToken.h>
#include <Lexical/Tokens/VarToken.h>
#include <Lexical/Tokens/ClosedBracketToken.h>
#include <Lexical/Tokens/OpenBracketToken.h>
#include <Lexical/Tokens/SallocToken.h>
#include <Lexical/Tokens/ModuloToken.h>
#include <Lexical/Tokens/SyscallToken.h>
#include <Lexical/Tokens/AssignToken.h>
#include <Lexical/Tokens/DeclToken.h>
#include <Lexical/Tokens/ExitToken.h>
#include <Lexical/Tokens/IndentToken.h>
#include <Lexical/Tokens/FunctionToken.h>
#include <Lexical/Tokens/ForToken.h>
#include <Lexical/Tokens/ElifToken.h>
#include <Lexical/Tokens/ColonToken.h>
#include <Lexical/Tokens/ExtentToken.h>
#include <Lexical/Tokens/CommaToken.h>
#include <Lexical/Tokens/ReturnToken.h>
#include <General/builtinTypes.h>
#include <Lexical/Tokens/IfToken.h>
#include <Lexical/Tokens/ComparisionToken.h>
#include <Lexical/Tokens/NotToken.h>
#include <Lexical/Tokens/ElseToken.h>
#include <Lexical/Tokens/ArrowToken.h>
#include <Lexical/Tokens/PlusToken.h>
#include <Lexical/Tokens/StarToken.h>
#include <Lexical/Tokens/MinusToken.h>
#include <Lexical/Tokens/OpenCurlyToken.h>
#include <Lexical/Tokens/ClosedCurlyToken.h>
#include <Lexical/Tokens/SlashToken.h>
#include <Lexical/Tokens/DotToken.h>
#include <Lexical/Tokens/DoubleQuoteToken.h>
#include <Lexical/Tokens/TextToken.h>
#include <Lexical/Tokens/QuoteToken.h>
#include <Lexical/Tokens/ImportToken.h>

bool contains(const std::string &str, std::vector<std::string> options){
    for(auto const & option : options){
        if(str == option)
            return true;
    }
    return false;
}

bool contains(char c, std::vector<std::string> options){
    for(auto const & option : options){
        if(c == option[0])
            return true;
    }
    return false;
}


ACC::LexicalAnalysis::LexicalAnalysis(std::string path){
    indentList.push_back(0);
    refCount++;
    std::ifstream fs;
    fs.open(path);
    this->document = std::string((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

    LOG.createHeading("Original Input being Lexically Analysed:");
    LOG() << this->document << std::endl;

    analyse();
    postProcessDocument();

}

ACC::LexicalAnalysis::~LexicalAnalysis() {
    refCount--;
    if(refCount != 0)
        return;

    for(const auto& it : tokens)
        delete it;
}

void ACC::LexicalAnalysis::postProcessDocument() {
    for(size_t i = 1; i < indentList.size(); i++){
        tokens.push_back(new ExtentToken(0));
    }
}

void ACC::LexicalAnalysis::printToken() {
    LOG.createHeading("Tokens generated by the Lexical Analysis:");
    int prefix = 0;
    std::string out;
    for(auto token : tokens){
        if(token->id == Symbol::INDENT)
            prefix += 4;
        else if (token->id == Symbol::EXTENT)
            prefix -= 4;
        if (prefix < 0)
            out += "[[[NEGATIVE INDEX]]]" + token->getIdentifier()+ "\n";
        else
            out += std::string(prefix, ' ') + token->getIdentifier()+ "\n";
    }
    LOG() << out;
    std::ofstream os ("./debug.txt");
    os << out;
    os.close();
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

void ACC::LexicalAnalysis::addZeroExit() {
    tokens.push_back(new ExitToken(0));
    tokens.push_back(new TextToken("0", 0));
    tokens.push_back(new EOSToken(0));
}

int ACC::LexicalAnalysis::readDepth(size_t& pos){
    int newGap = 0;
    while(contains(document.at(pos), {" ", "\n", "\r"})){
        if(document.at(pos) == '\n'){
            newGap = 0;
        }else if(document.at(pos) == ' ')
            newGap++;
        ++pos;
        if(pos >= document.size())
            return 0;
    }
    return newGap;
}

void ACC::LexicalAnalysis::analyse() {
    size_t idx = 0;
    size_t lineNum = 1;
    std::string buffer;
    bool shallCheckIndent = false;

    while(idx < document.size()){

        if(contains(document[idx], {"\n","\r"})){
            idx++;
            lineNum++;
            checkIndent(idx, lineNum);
            continue;
        }

        buffer = loadBuffer(idx);
        idx++;

        if(buffer[0] == ' ')
            buffer = buffer.substr(1, buffer.size());


        if(checkSpecial(buffer, 0))
            continue;

        if(checkKeyword(buffer, 0))
            continue;

        if(!buffer.empty())
            tokens.push_back(new TextToken(buffer, lineNum));

    }
}

void ACC::LexicalAnalysis::checkIndent(size_t &idx, size_t lineNum) {
    int newDepth = readDepth(idx);
    if (newDepth > depth){
        tokens.push_back(new IndentToken(lineNum));
        indentList.push_back(newDepth);
    }
    else if (newDepth < depth){
        while( indentList.at(indentList.size() - 1) > newDepth  ){
            tokens.push_back(new ExtentToken(lineNum));
            indentList.pop_back();
        }
        //popScope();
    }
    depth = newDepth;
}

bool ACC::LexicalAnalysis::checkSpecial(const std::string &buffer, size_t lineNum) {
    if (buffer == "!=")
        tokens.push_back(new ComparisionToken(ComparisionTokenKind::NotEqual, lineNum));

    else if (buffer == "<=")
        tokens.push_back(new ComparisionToken(ComparisionTokenKind::LessEqual, lineNum));

    else if (buffer == ">=")
        tokens.push_back(new ComparisionToken(ComparisionTokenKind::GreaterEqual, lineNum));

    else if (buffer == "==")
        tokens.push_back(new ComparisionToken(ComparisionTokenKind::Equal, lineNum));

    else if (buffer == "->")
        tokens.push_back(new ArrowToken(lineNum));

    else if (buffer == ":")
        tokens.push_back(new ColonToken(lineNum));

    else if (buffer == ";")
        tokens.push_back(new EOSToken(lineNum));

    else if (buffer == "{")
        tokens.push_back(new OpenCurlyToken(lineNum));

    else if (buffer == "}")
        tokens.push_back(new ClosedCurlyToken(lineNum));

    else if (buffer  == "!")
        tokens.push_back(new NotToken(lineNum));

    else if (buffer  == ".")
        tokens.push_back(new DotToken(lineNum));

    else if (buffer  == "%")
        tokens.push_back(new ModuloToken(lineNum));

    else if(buffer  == "<")
        tokens.push_back(new ComparisionToken(ComparisionTokenKind::Less, lineNum));

    else if(buffer  == ">")
        tokens.push_back(new ComparisionToken(ComparisionTokenKind::Greater, lineNum));

    else if(buffer  == "\"")
        tokens.push_back(new DoubleQuoteToken(lineNum));

    else if(buffer  == "'")
        tokens.push_back(new QuoteToken(lineNum));

    else if (buffer  == ",")
        tokens.push_back(new CommaToken(lineNum));

    else if (buffer  == "(")
        tokens.push_back(new OpenBracketToken(lineNum));

    else if (buffer  == ")")
        tokens.push_back(new ClosedBracketToken(lineNum));

    else if (buffer  == "+")
        tokens.push_back(new PlusToken(lineNum));

    else if (buffer  == "-")
        tokens.push_back(new MinusToken(lineNum));

    else if (buffer  == "*")
        tokens.push_back(new StarToken(lineNum));

    else if (buffer  == "/")
        tokens.push_back(new SlashToken(lineNum));

    else if (buffer  == ",")
        tokens.push_back(new CommaToken(lineNum));

    else if (buffer  == "=")
        tokens.push_back(new AssignToken(lineNum));

    else
        return false;

    return true;
}

bool ACC::LexicalAnalysis::checkKeyword(std::string const &buffer, size_t lineNum) {
    if(buffer == "fn")
        tokens.push_back(new FunctionToken(lineNum));

    else if (buffer == "else")
        tokens.push_back(new ElseToken(lineNum));

    else if (buffer == "exit")
        tokens.push_back(new ExitToken(lineNum));

    else if (buffer == "for")
        tokens.push_back(new ForToken(lineNum));

    else if (buffer == "if")
        tokens.push_back(new IfToken(lineNum));

    else if (buffer == "import")
        tokens.push_back(new ImportToken(lineNum));

    else if (buffer == "return")
        tokens.push_back(new ReturnToken(lineNum));

    else if (buffer == "salloc")
        tokens.push_back(new SallocToken(lineNum));

    else if (buffer == "syscall")
        tokens.push_back(new SyscallToken(lineNum));

    else if (buffer == "var")
        tokens.push_back(new VarToken(lineNum));

    else if (buffer == "while")
        tokens.push_back(new WhileToken(lineNum));

    else
        return false;

    return true;
}

std::string ACC::LexicalAnalysis::loadBuffer(size_t &idx) {
    const std::vector<std::string> specialTokens = {"\"", ";", " ", "\n", "\r", "(", ")", "+", "-", "*", "/", ",", "=",
                                                    "<", ">", "!", ":", "\'", "\"", "%", ",", "{", "}", "."};


    std::string buffer;
    if(contains(document[idx], specialTokens)) {

        buffer += document[idx];

        // Most special tokens that can be grouped are only on character (e.g. (, !, " ) wide, we do need to check for
        // some exceptions though.

        if(idx + 1 >= document.size())
            return buffer;

        const std::string toTest = std::string() + document[idx] + document[idx + 1];

        if (toTest == "->") {
            buffer = "->";
            idx++;
        } else if (toTest == "<=") {
            buffer = "<=";
            idx++;

        } else if (toTest == ">=") {
            buffer = ">=";
            idx++;
        } else if (toTest == "==") {
            buffer = "==";
            idx++;
        }
    }

    else{
        while(idx < document.size() && !contains(document[idx], specialTokens)){
            buffer += document[idx];
            idx++;

        }
        idx--;
    }

    return buffer;
}

ACC::LexicalAnalysis::LexicalAnalysis(const ACC::LexicalAnalysis &other) : tokens(other.tokens), document(other.document),
refCount(other.refCount), indentList(other.indentList), depth(other.depth){
    refCount++;
}