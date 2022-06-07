
#include "Copy.h"
#include "Add.h"
#include "Variable.h"
#include "Subtract.h"
#include "Multiply.h"
#include "Divide.h"
#include "Scalar.h"


Expression& Copy::Get()
{
    return *last;
}

void Copy::SetReplacement(Expression& replace, Expression& replacement)
{
    done = false;
    this->replace = &replace;
    this->replacement = &replacement;
}

bool Copy::CheckReplace(Expression &ex)
{
    if (done)
    {
        last =&ex;
    }
    else if (&ex == replace && replacement)
    {
        last = replacement;
        done = true;
    }
    return done;
}

bool Copy::Visit(Sum& add)
{
    if (CheckReplace(add))
    {
        return false;
    }

    std::vector<Expression*> newValues;
    for (int i = 0; i < add.values.size(); ++i)
    {
        add.values[i]->Walk(*this);
        newValues.push_back(&Get());
    }

    //do copy
    last = &factoryFacade.sumFactory.Create(newValues);

    return false;
}

bool Copy::Visit(Add& add)
{
    if (CheckReplace(add))
    {
        return false;
    }

    add.lhs.Walk(*this);
    Expression& newLhs = Get();
    add.rhs.Walk(*this);
    Expression& newRhs = Get();

    //do copy
    last = &factoryFacade.addFactory.Create(newLhs, newRhs);

    return false;
}
bool Copy::Visit(Subtract& sub)
{
    if (CheckReplace(sub))
    {
        return false;
    }

    sub.lhs.Walk(*this);
    Expression &newLhs = Get();
    sub.rhs.Walk(*this);
    Expression &newRhs = Get();

    //do copy
    last = &factoryFacade.subtractFactory.Create(newLhs, newRhs);

    return false;
}
bool Copy::Visit(Products& mult)
{
    if (CheckReplace(mult))
    {
        return false;
    }

    std::vector<Expression*> newValues;
    for (int i = 0; i < mult.values.size(); ++i)
    {
        mult.values[i]->Walk(*this);
        newValues.push_back(&Get());
    }

    //do copy
    last = &factoryFacade.productsFactory.Create(newValues);

    return false;
}
bool Copy::Visit(Multiply& mult)
{
    if (CheckReplace(mult))
    {
        return false;
    }

    mult.lhs.Walk(*this);
    Expression &newLhs = Get();
    mult.rhs.Walk(*this);
    Expression &newRhs = Get();

    //do copy
    last = &factoryFacade.multiplyFactory.Create(newLhs, newRhs);

    return false;
}
bool Copy::Visit(Divide& divide)
{
    if (CheckReplace(divide))
    {
        return false;
    }

    divide.lhs.Walk(*this);
    Expression &newLhs = Get();
    divide.rhs.Walk(*this);
    Expression &newRhs = Get();

    //do copy
    last = &factoryFacade.divideFactory.Create(newLhs, newRhs);


    return false;
}
bool Copy::Visit(Variable& var)
{
    last = &var;
    return false;
}
bool Copy::Visit(Scalar& scalar)
{
    last = &scalar;
    return false;
}
bool Copy::Visit(Expression& e)
{
    return false;
}