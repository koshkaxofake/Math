#pragma once

#include "Expression.h"
#include "Visitor.h"

class Add : public Expression
{
public:
    Add(Expression & lhs, Expression & rhs) : lhs(lhs), rhs(rhs)
    {

    }
    bool operator==(const Expression& other) const
    {
        if (this == &other)
            return true;
        const Add* otherAdd = dynamic_cast<const Add*>(&other);
        if (!otherAdd)
            return false;
        return lhs == otherAdd->lhs && rhs == otherAdd->rhs;
    }
    void Walk(Visitor &v)
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