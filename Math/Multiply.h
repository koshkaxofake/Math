#pragma once

#include "Expression.h"
#include "Visitor.h"

class Multiply : public Expression
{
public:
    Multiply(Expression& lhs, Expression& rhs) : lhs(lhs), rhs(rhs)
    {

    }
    bool operator==(const Expression& other) const
    {
        if (this == &other)
            return true;
        const Multiply* otherMultiply = dynamic_cast<const Multiply*>(&other);
        if (!otherMultiply)
            return false;
        return lhs == otherMultiply->lhs && rhs == otherMultiply->rhs;
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