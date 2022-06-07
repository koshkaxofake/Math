#pragma once

#include "Subtract.h"
#include <list>

class SubtractFactory
{
public:
    Subtract& Create(Expression& lhs, Expression& rhs)
    {
        mem.emplace_front(lhs, rhs);
        return mem.front();
    }
    std::list<Subtract> mem;
};