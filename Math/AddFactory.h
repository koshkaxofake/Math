#pragma once

#include "Add.h"
#include <list>

class AddFactory
{
public:
    Add& Create(Expression &lhs, Expression &rhs)
    {
        mem.emplace_front(lhs, rhs);
        return mem.front();
    }
    std::list<Add> mem;
};