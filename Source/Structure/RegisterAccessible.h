//
// Created by a_mod on 12.06.2019.
//
#pragma once

#include <Structure/Structure.h>

namespace ACC {
    class RegisterAccessible{
    public:
        virtual Register getRegister() const = 0;
    };
}