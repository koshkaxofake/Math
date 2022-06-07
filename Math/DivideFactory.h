#pragma once

#include "Divide.h"
#include <list>

class DivideFactory
{
public:
    Divide& Create(Expression& lhs, Expression& rhs)
    {
        mem.emplace_front(lhs, rhs);
        return mem.front();
    }
    std::list<Divide> mem;
};