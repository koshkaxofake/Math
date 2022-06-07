#pragma once

#include "Expression.h"
#include "Visitor.h"

class Subtract : public Expression
{
public:
    Subtract(Expression& lhs, Expression& rhs) : lhs(lhs), rhs(rhs)
    {

    }
    bool operator==(const Expression& other) const
    {
        if (this == &other)
            return true;
        const Subtract* otheSubtract = dynamic_cast<const Subtract*>(&other);
        if (!otheSubtract)
            return false;
        return lhs == otheSubtract->lhs && rhs == otheSubtract->rhs;
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