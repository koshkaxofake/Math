#pragma once

#include "Multiply.h"
#include <list>

class MultiplyFactory
{
public:
    Multiply& Create(Expression& lhs, Expression& rhs)
    {
        mem.emplace_front(lhs, rhs);
        return mem.front();
    }
    std::list<Multiply> mem;
};