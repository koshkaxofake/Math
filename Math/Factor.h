#pragma once

#include "Multiply.h"
#include <list>

// ax + bx  -> (a+b) * x
class Factor
{
public:

    Expression& Apply(Multiply& mult, Multiply& mult2);
private:
    //store all the expressions made by the factor
    std::list<Add> adds;
    std::list<Multiply> mults;
};