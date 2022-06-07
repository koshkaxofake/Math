#include "ScalarMath.h"
#include "Add.h"
#include "Multiply.h"

Expression& ScalarMath::Apply(Expression& e)
{
    replacement = 0;
    e.Walk(*this);
    if (replace)
    {
        if (replacement == nullptr)
            replacement = &facade.scalarFactory.Create(value);
        copy.SetReplacement(*replace, *replacement);
        e.Walk(copy);
        return copy.Get();
    }
    return e;
}

bool ScalarMath::Default(Expression& e)
{
    allScalar = false;
    return true;
}

bool ScalarMath::Visit(Add& add)
{
    add.lhs.Walk(*this);
    if (!allScalar && replace)
        return false;
    Rational lhs = value;
    add.rhs.Walk(*this);
    if (allScalar)
    {
        value = lhs + value;
        replace = &add;
    }
    
    return false;
}

bool ScalarMath::Visit(Sum& sum)
{
    Rational total(0);
    int num = 0;
    std::vector<Expression*> nonScalar;
    for (int i = 0; i < sum.values.size(); ++i)
    {
        sum.values[i]->Walk(*this);
        if (!allScalar && replace)
        {
            allScalar = true;
            nonScalar.push_back(sum.values[i]);
            continue;
        }
        if (allScalar)
        {
            total += value;
            num += 1;
        }
    }
    if (num == sum.values.size())
    {
        //all values were scalar
        value = total;
        allScalar = true;
        replace = &sum;
    }
    else if (num >= 2)
    {
        //if at least two values are scalar we can combine those and make a new sum
        Scalar &combined = facade.scalarFactory.Create(total);
        nonScalar.push_back(&combined);
        Sum& res = facade.sumFactory.Create(nonScalar);
        replace = &sum;
        replacement = &res;
    }

    return false;
}

bool ScalarMath::Visit(Multiply& mult)
{
    mult.lhs.Walk(*this);
    if (!allScalar && replace)
        return false;
    Rational lhs = value;
    mult.rhs.Walk(*this);
    if (allScalar)
    {
        value = lhs * value;
        replace = &mult;
    }

    return false;
}


bool ScalarMath::Visit(Subtract& sub)
{
    sub.lhs.Walk(*this);
    if (!allScalar && replace)
        return false;
    Rational lhs = value;
    sub.rhs.Walk(*this);
    if (allScalar)
    {
        value = lhs - value;
        replace = &sub;
    }

    return false;
}

bool ScalarMath::Visit(Divide& div)
{
    div.lhs.Walk(*this);
    if (!allScalar && replace)
        return false;
    Rational lhs = value;
    div.rhs.Walk(*this);
    if (allScalar)
    {
        value = lhs / value;
        replace = &div;
    }

    return false;
}

bool ScalarMath::Visit(Scalar &scalar)
{
    value = scalar.value;
    allScalar = true;
    return true;
}