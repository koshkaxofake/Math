#pragma once

#include "Sum.h"
#include <list>
#include <vector>

class SumFactory
{
public:
    Sum& Create(std::vector<Expression*> sums)
    {
        mem.emplace_front(sums);
        return mem.front();
    }
    std::list<Sum> mem;
};