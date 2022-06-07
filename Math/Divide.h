#pragma once

#include "Expression.h"
#include "Visitor.h"

class Divide : public Expression
{
public:
    bool operator==(const Expression& other) const
    {
        if (this == &other)
            return true;
        const Divide* otherDivide= dynamic_cast<const Divide*>(&other);
        if (!otherDivide)
            return false;
        return lhs == otherDivide->lhs && rhs == otherDivide->rhs;
    }
    Divide(Expression& lhs, Expression& rhs) : lhs(lhs), rhs(rhs)
    {

    }
    void Walk(Visitor& v)
    {
        if (v.Visit(*this))
        {
            lhs.Walk(v);
            rhs.Walk(v);
        }
    }
    Expression& lhs;
    Expression& rhs;
};