#include "FactorPattern.h"
#include "Add.h"
#include "Multiply.h"
#include <unordered_map>

Expression &FactorPattern::Apply(Expression &e)
{
    found = false;
    e.Walk(*this);
    if (found)
    {
        e.Walk(copy);
        return copy.Get();
    }
    return e;
}
bool FactorPattern::Visit(Add& add)
{
    FactorChecker fc;
    if (fc.Check(add))
    {
        copy.SetReplacement(add, factor.Apply(*fc.lhs, *fc.rhs));
        found = true;
        return false;
    }
    else
    {
        return true;
    }
}
bool FactorPattern::Visit(Sum& sum)
{
    FactorCheckerSum fc(facade);
    if (fc.Check(sum))
    {
        copy.SetReplacement(sum, *fc.result);
        found = true; 
        return false;
    }
    else
    {
        return true;
    }
}
bool FactorPattern::FactorChecker::Check(Add& e)
{
    lhs = nullptr;
    rhs = nullptr;
    e.lhs.Walk(*this);
    e.rhs.Walk(*this);
    return lhs && rhs && (lhs->lhs == rhs->rhs || lhs->lhs == rhs->lhs || lhs->rhs == rhs->rhs || lhs->rhs == rhs->lhs);
}
bool FactorPattern::FactorChecker::Visit(Multiply& mult)
{
    if (lhs)
    {
        rhs = &mult;
    }
    else
    {
        lhs = &mult;
    }
    return false;
}
bool FactorPattern::FactorChecker::Default(Expression& e)
{
    return false;
}

bool FactorPattern::FactorCheckerSum::Check(Sum& e)
{
    result = nullptr;
    values.clear();
    for (int i = 0; i < e.values.size(); ++i)
    {
        e.values[i]->Walk(*this);
    }

    if (values.size() >= 2)
    {
        std::vector<Expression*> factors;
        for (int i = 0; i < values.size(); ++i)
        {
            factors.push_back(&values[i]->lhs);
            factors.push_back(&values[i]->rhs);
        }
        //ax + by + cx -> (a+c)x + y
        //ax + bx + cx  -> (a + b + c) * x
        //ax + bx + c   ->  (a + b) * x + c
        //ax + b + c->no factor
        std::unordered_map<Expression*, std::vector<Expression*>> factorCount;
        for (int i = 0; i < values.size(); ++i)
        {
            factorCount[&values[i]->lhs].push_back(&values[i]->rhs);
            factorCount[&values[i]->rhs].push_back(&values[i]->lhs);
        }
        // ax + by {x=y}
        for (int i = 0; i < factors.size(); ++i)
        {
            for (int j = i + 1; j < factors.size(); ++j)
            {
                if (*factors[i] == *factors[j] && factors[i] != factors[j])
                {
                    std::vector<Expression*> temp;
                    for (int ii = 0; ii < factorCount[factors[i]].size(); ++ii)
                    {
                        temp.push_back(factorCount[factors[i]][ii]);
                    }
                    for (int ii = 0; ii < factorCount[factors[j]].size(); ++ii)
                    {
                        temp.push_back(factorCount[factors[j]][ii]);
                    }
                    factorCount[factors[i]] = temp;
                    factorCount[factors[j]] = temp;
                }
            }
        }

        // ax + bx + ay + by -> (a+b)*x + ay + by or (a+b)*x (a+b)*y -> (a+b)*(x+y)
        //shared will be the most common factor
        Expression* shared = &values[0]->lhs;
        for (int i = 0; i < values.size(); ++i)
        {
            if (factorCount[&values[i]->lhs].size() > factorCount[shared].size())
            {
                shared = &values[i]->lhs;
            }
            if (factorCount[&values[i]->rhs].size() > factorCount[shared].size())
            {
                shared = &values[i]->rhs;
            }
        }

        if (factorCount[shared].size() >= 2)
        {
            //create new sum and multiply
            Sum& sum = facade.sumFactory.Create(factorCount[shared]);
            Multiply& res = facade.multiplyFactory.Create(sum, *shared);
            //get the expression from the original sum that arent being factored
            std::vector<Expression*> notFactored;
            //add the expressions that wern multiplied
            for (int i = 0; i < e.values.size(); ++i)
            {
                if (std::find(values.begin(), values.end(), e.values[i]) == values.end())
                {
                    notFactored.push_back(e.values[i]);
                }
            }
            //add the multiply espressions that didnt have the shared factor
            for (int i = 0; i < values.size(); ++i)
            {
                if (values[i]->lhs != *shared && values[i]->rhs != *shared)
                {
                    notFactored.push_back(values[i]);
                }
            }
            //create the final sum
            result = &res;
            if (notFactored.size() > 0)
            {
                notFactored.push_back(&res);
                result = &facade.sumFactory.Create(notFactored);
            }
        }
    }
    return result;
}
bool FactorPattern::FactorCheckerSum::Visit(Multiply& mult)
{
    values.push_back(&mult);
    return false;
}
bool FactorPattern::FactorCheckerSum::Default(Expression& e)
{
    return false;
}