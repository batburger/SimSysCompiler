//
// Created by a_mod on 10.02.2019.
//

#ifndef SIMSYSCOMPILER_CODE_H
#define SIMSYSCOMPILER_CODE_H

#include <vector>
#include <string>
#include <IntermediateCodeGenerator/Operator.h>
#include <unordered_map>

namespace ACC{

    struct Dependency{
        temporary temp = 0;
        Operator* op = nullptr;
    };

    class Code {
    private:
        std::vector<Operator*> data;
        std::unordered_map<std::string, Dependency> symTable;
        temporary temporaryCounter = 1;

        std::string operator2String(Operator op);
        std::string printAsTemporary(ACC::temporary temp);


    public:
        Code();
        Dependency& getSymbol(std::string sym);
        Dependency& emplaceSymbol(std::string sym, Operator* op);
        void pushOp(Operator *const &op);
        void removeUnary(size_t idx);
        Dependency createTemporary();
        Operator* at(size_t idx);

        std::vector<Operator*>::iterator begin();
        std::vector<Operator*>::iterator end();

        std::vector<Operator*>& getData();
        void print();

    };
}

#endif //SIMSYSCOMPILER_CODE_H
