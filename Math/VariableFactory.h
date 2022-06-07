#pragma once

#include "Variable.h"
#include <list>

class VariableFactory
{
public:
    Variable& Create()
    {
        mem.emplace_front();
        return mem.front();
    }
    std::list<Variable> mem;
};