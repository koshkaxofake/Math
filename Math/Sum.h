#pragma once

#include "Expression.h"
#include "Visitor.h"
#include <vector>

class Sum : public Expression
{
public:
    Sum(Expression& lhs, Expression& rhs)
    {
        values.push_back(&lhs);
        values.push_back(&rhs);
    }
    Sum(std::vector<Expression*> arg) :values(arg)
    {
    }
    bool operator==(const Expression& other) const
    {
        if (this == &other)
            return true;
        const Sum* otherAdd = dynamic_cast<const Sum*>(&other);
        if (!otherAdd || otherAdd->values.size() != values.size())
            return false;

        //need checked array in case of dups like the example below
        // a + b + b == b + a + a 
        std::vector<bool> checked(values.size(), false);
        for (int i = 0; i < values.size(); ++i)
        {
            for (int j = 0; j < otherAdd->values.size(); ++j)
            {
                if (checked[j])
                    continue;
                checked[j] = values[i] == otherAdd->values[j];
                if (checked[j])
                    break;
            }
        }

        bool res = true;
        for (int i = 0; i < checked.size(); ++i)
            res &= checked[i];
        return res;
    }
    void Walk(Visitor& v)
    {
        if (v.Visit(*this))
        {
            for (int i = 0; i < values.size(); ++i)
            {
                values[i]->Walk(v);
            }
        }
    }
    //these will always be valid pointers, so no need to check
    std::vector<Expression*> values;
};