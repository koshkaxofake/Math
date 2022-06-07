#pragma once

#include "Scalar.h"
#include <list>

class ScalarFactory
{
public:
    Scalar& Create(Rational value)
    {
        mem.emplace_front(value);
        return mem.front();
    }
    std::list<Scalar> mem;
};