#pragma once

#include "Expression.h"
#include "Visitor.h"

class Pattern : public Visitor
{
public:
    virtual Expression& Apply(Expression &e) = 0;
};