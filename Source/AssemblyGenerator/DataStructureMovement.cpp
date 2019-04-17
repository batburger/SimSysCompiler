//
// Created by a_mod on 16.04.2019.
//

#include "DataStructureMovement.h"
#include <utils.h>

std::string ACC::Movs::c2st(Location constant){
    if(constant.accessMethod != AccessMethod::CONSTANT)
        throw std::runtime_error("Called data structure movement method for movement of constants with `Location` objects"
                                 "not containing constants.");


    std::string value = constant.constant;
    std::string str = "mov dword [rsp], 0x";
    size_t count = 0;

    for (size_t i = 0; i < value.size(); i++) {
            size_t offset = str.size();
            if (count == 4) {
                str += "\nmov dword [rsp + " + std::to_string(i) + "], 0x";
                offset = str.size();
                count = 0;
            }

            str.insert(offset, toHex((unsigned)value.at(i)));
            count++;
    }
    return str;
}

std::string ACC::Movs::bp2st(ACC::Location where, Stack<size_t> &structure) {
    if(where.accessMethod != AccessMethod::SBP_OFFSET)
        throw std::runtime_error("Called data structure movement method for movement of data stored as an offset from "
                                 "the stack base pointer with a `Location` object not containing such an structure.");

    std::string out;
    auto offset = where.offsetInfo;
    size_t size = structure.pop();

    for(size_t i = 0; i < size; i++){
        out += "\nmov byte al, [rbp + "+std::to_string(offset)+"]";
        out += "\nmov byte [rsp + "+std::to_string(i)+"], al";
        offset += 1;
    }
    return out;
}

std::string ACC::Movs::c2bp(Location constant, ACC::Location where) {
    auto bpOffset = where.offsetInfo;
    auto sign = std::string(bpOffset >= 0 ? ("+") : ("-"));
    std::string str = "mov dword [rbp "+sign+" "+std::to_string(bpOffset)+"], 0x";
    size_t count = 0;
    std::string value = constant.constant;

    for (size_t i = 0; i < value.size(); i++) {
        size_t offset = str.size();
        if (count == 4) {
            str += "\nmov dword [rbp "+sign+" "+std::to_string(bpOffset + i)+"], 0x";
            offset = str.size();
            count = 0;
        }

        str.insert(offset, toHex((unsigned)value.at(i)));
        count++;
    }

    return str;
}

std::string ACC::Movs::r2bp(ACC::Location where, ACC::Location reg) {
    // TODO: Utterly ignores sizes.

    return "mov [rbp + " + std::to_string(where.offsetInfo) + "], "+registerToString(reg.regInfo);
}

std::string ACC::Movs::c2so(ACC::Location constant, ACC::Location where) {
    auto spOffset = where.offsetInfo;
    auto sign = std::string(spOffset >= 0 ? ("+") : ("-"));

    std::string str = "mov dword [rsp " + sign + " " + std::to_string(spOffset)+"], 0x";
    size_t count = 0;
    std::string value = constant.constant;

    for (size_t i = 0; i < value.size(); i++) {
        size_t offset = str.size();
        if (count == 4) {
            str += "\nmov dword [rsp " + sign +" "+std::to_string(spOffset + i)+"], 0x";
            offset = str.size();
            count = 0;
        }

        str.insert(offset, toHex((unsigned)value.at(i)));
        count++;
    }
    return str;
}

std::string ACC::Movs::r2st(ACC::Location reg) {
    // TODO: Utterly ignores sizes.

    return "mov [rsp], " + registerToString(reg.regInfo);
}

