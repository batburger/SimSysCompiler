#include "PtrLValueStructure.h"

#include <utility>
#include <Assembly/Code.h>


ACC::PtrLValueStructure::PtrLValueStructure(std::string access, ACC::Type type)
        : PtrStructure(ValueCategory::lvalue, type), access(std::move(access))
{

}

std::string const &ACC::PtrLValueStructure::getAccess() {
    return access;
}

void ACC::PtrLValueStructure::loadToRegister(ACC::Register reg, ACC::Code &code) {
    auto& fn = code.getFnSymbol();
    fn.writeLine("mov " + registerToString(8, reg) + ", [" + access + "]");
}

std::shared_ptr<ACC::Structure>
ACC::PtrLValueStructure::operatorCopy(std::shared_ptr<ACC::Structure> address, ACC::Code & code) {
    if(address->vCategory == ValueCategory::lvalue) {
        auto &fn = code.getFnSymbol();
        auto *addressAsLValue = dynamic_cast<GenericLValueStructure *>(address.get());

        Register reg = code.getFreeRegister();
        std::string regStr = registerToString(8, reg);

        fn.writeLine("mov " + regStr + ", [" + access + "]");
        fn.writeLine("mov [ " + addressAsLValue->getAccess() + " ], " + regStr);

        return address;
    }
    return nullptr;
}
