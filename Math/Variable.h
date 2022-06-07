#pragma once

#include "Expression.h"
#include "Visitor.h"

class Variable : public Expression
{
public:
    Variable()
    {

    }
    void Walk(Visitor& v)
    {
        v.Visit(*this);
    }
private:
};