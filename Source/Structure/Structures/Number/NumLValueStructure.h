//
// Created by a_mod on 03.06.2019.
//

#ifndef SIMSYSCOMPILER_NUMBUILTINTYPE_H
#define SIMSYSCOMPILER_NUMBUILTINTYPE_H

#include <Structure/Structure.h>
#include "Structure/Structures/ElementaryStructure.h"
#include "NumStructure.h"
#include "Structure/Structures/GenericLValueStructure.h"

namespace ACC{
    class NumLValueStructure : public NumStructure, public AsmAccessible{
    protected:
        std::string access;
    public:
        explicit NumLValueStructure(std::string const & access);

        void loadToRegister(Register reg, Code& code) override;
        std::shared_ptr<Structure> operatorCopy(std::vector<std::shared_ptr<Structure>> objList, Code &) override;
        std::string const & getAccess() const override;

        std::shared_ptr<Structure> operatorChar(Code & code) override;
        std::shared_ptr<Structure> operatorNum(Code & code) override;
        std::shared_ptr<Structure> operatorBool(Code & code) override;
        std::shared_ptr<Structure> operatorPtr(Code &code, Type pointingTo) override;

        bool hasConversionTo(const Type &id) override;
    };
}

#endif //SIMSYSCOMPILER_NUMBUILTINTYPE_H
