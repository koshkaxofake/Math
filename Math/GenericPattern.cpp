#include "GenericPattern.h"
#include "Add.h"
#include "Multiply.h"
#include <unordered_map>

Expression& GenericPattern::Apply(Expression& e)
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
bool GenericPattern::Default(Expression& e)
{
    Checker c(matcher);
    if (c.Check(e))
    {
        //create the resulting expression
        Creator creator(c.map, facade);
        result.Walk(creator);
        copy.SetReplacement(e, *creator.returnValue);
        found = true;
        return Continue;
    }
    else
    {
        return Stop;
    }
}
bool GenericPattern::Checker::Check(Expression& e)
{
    current = &matcher;
    map.clear();
    failed = false;
    e.Walk(*this);
    return !failed;
}
bool GenericPattern::Checker::Visit(Add& e)
{
    Add* matchE;
    if (matchE = dynamic_cast<Add*>(current))
    {
        map[current] = &e;
        current = &matchE->lhs;
        e.lhs.Walk(*this);
        current = &matchE->rhs;
        e.rhs.Walk(*this);
    }
    else if (dynamic_cast<Variable*>(current) && (map.count(current) == 0 || *map[current] == e))
    {
        map[current] = &e;
    }
    else
    {
        failed = true;
    }
    return Stop;
}
bool GenericPattern::Checker::Visit(Multiply& e)
{
    Multiply* matchE;
    if (matchE = dynamic_cast<Multiply*>(current))
    {
        map[current] = &e;
        current = &matchE->lhs;
        e.lhs.Walk(*this);
        current = &matchE->rhs;
        e.rhs.Walk(*this);
    }
    else if (dynamic_cast<Variable*>(current) && (map.count(current) == 0 || *map[current] == e))
    {
        map[current] = &e;
    }
    else
    {
        failed = true;
    }
    return Stop;
}

bool GenericPattern::Checker::Visit(Subtract& e)
{
    Subtract* matchE;
    if (matchE = dynamic_cast<Subtract*>(current))
    {
        map[current] = &e;
        current = &matchE->lhs;
        e.lhs.Walk(*this);
        current = &matchE->rhs;
        e.rhs.Walk(*this);
    }
    else if (dynamic_cast<Variable*>(current) && (map.count(current) == 0 || *map[current] == e))
    {
        map[current] = &e;
    }
    else
    {
        failed = true;
    }
    return true;
}

bool GenericPattern::Checker::Visit(Divide& e)
{
    Divide* matchE;
    if (matchE = dynamic_cast<Divide*>(current))
    {
        map[current] = &e;
        current = &matchE->lhs;
        e.lhs.Walk(*this);
        current = &matchE->rhs;
        e.rhs.Walk(*this);
    }
    else if (dynamic_cast<Variable*>(current) && (map.count(current) == 0 || *map[current] == e))
    {
        map[current] = &e;
    }
    else
    {
        failed = true;
    }
    return Stop;
}

bool GenericPattern::Checker::Visit(Variable& e)
{
    Variable* matchE;
    if (matchE = dynamic_cast<Variable*>(current))
    {
        if (map.count(current) && *map[current] != e)
        {
            //not a match
            failed = true;
        }
        map[current] = &e;
    }
    else
    {
        failed = true;
    }
    return Stop;
}

bool GenericPattern::Checker::Visit(Scalar& e)
{
    Scalar* matchE;
    if (matchE = dynamic_cast<Scalar*>(current))
    {
        if (map.count(current) && *map[current] != e)
        {
            //not a match
            failed = true;
        }
        map[current] = &e;
    }
    else if (dynamic_cast<Variable*>(current) && (map.count(current) == 0 || *map[current] == e))
    {
        map[current] = &e;
    }
    else
    {
        failed = true;
    }
    return Stop;
}

bool GenericPattern::Checker::Visit(Sum& e)
{
    Sum* matchE;
    if ((matchE = dynamic_cast<Sum*>(current)) && matchE->values.size() == e.values.size())
    {
        map[current] = &e;
        for (int i = 0; i < e.values.size(); ++i)
        {
            current = matchE->values[i];
            e.values[i]->Walk(*this);
        }
    }
    else if (dynamic_cast<Variable*>(current) && (map.count(current) == 0 || *map[current] == e))
    {
        map[current] = &e;
    }
    else
    {
        failed = true;
    }
    return Stop;
}

bool GenericPattern::Checker::Visit(Products& e)
{
    Sum* matchE;
    if ((matchE = dynamic_cast<Sum*>(current)) && matchE->values.size() == e.values.size())
    {
        map[current] = &e;
        for (int i = 0; i < e.values.size(); ++i)
        {
            current = matchE->values[i];
            e.values[i]->Walk(*this);
        }
    }
    else if (dynamic_cast<Variable*>(current) && (map.count(current) == 0 || *map[current] == e))
    {
        map[current] = &e;
    }
    else
    {
        failed = true;
    }
    return Stop;
}

bool GenericPattern::Checker::Default(Expression& e)
{
    //this functon shouldnt be called, assume it failed
    failed = true;
    return Stop;
}

//////////// Creator class Functions ///////////////
bool GenericPattern::Creator::Visit(Add& e)
{
    e.lhs.Walk(*this);
    Expression *lhs = returnValue;
    e.rhs.Walk(*this);
    Expression* rhs = returnValue;
    returnValue = &facade.addFactory.Create(*lhs, *rhs);
    return Stop;
}
bool GenericPattern::Creator::Visit(Multiply& e)
{
    e.lhs.Walk(*this);
    Expression* lhs = returnValue;
    e.rhs.Walk(*this);
    Expression* rhs = returnValue;
    returnValue = &facade.multiplyFactory.Create(*lhs, *rhs);
    return Stop;
}

bool GenericPattern::Creator::Visit(Subtract& e)
{
    e.lhs.Walk(*this);
    Expression* lhs = returnValue;
    e.rhs.Walk(*this);
    Expression* rhs = returnValue;
    returnValue = &facade.subtractFactory.Create(*lhs, *rhs);
    return Stop;
}

bool GenericPattern::Creator::Visit(Divide& e)
{
    e.lhs.Walk(*this);
    Expression* lhs = returnValue;
    e.rhs.Walk(*this);
    Expression* rhs = returnValue;
    returnValue = &facade.divideFactory.Create(*lhs, *rhs);
    return Stop;
}

bool GenericPattern::Creator::Visit(Variable& e)
{
    returnValue = map[&e];
    return Stop;
}

bool GenericPattern::Creator::Visit(Scalar& e)
{
    if (map.count(&e))
    {
        returnValue = map[&e];
    }
    else
    {
        returnValue = &e;
    }
    return Stop;
}

bool GenericPattern::Creator::Visit(Sum& e)
{
    std::vector<Expression*> values;
    for (int i = 0; i < e.values.size(); ++i)
    {
        e.values[i]->Walk(*this);
        values.push_back(returnValue);
    }
    returnValue = &facade.sumFactory.Create(values);
    return Stop;
}

bool GenericPattern::Creator::Visit(Products& e)
{
    std::vector<Expression*> values;
    for (int i = 0; i < e.values.size(); ++i)
    {
        e.values[i]->Walk(*this);
        values.push_back(returnValue);
    }
    returnValue = &facade.productsFactory.Create(values);
    return Stop;
}

bool GenericPattern::Creator::Default(Expression& e)
{
    //this functon shouldnt be called
    return Stop;
}