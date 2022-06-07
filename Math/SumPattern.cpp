#include "SumPattern.h"
#include "Add.h"
#include "Multiply.h"
#include <unordered_map>

Expression& SumPattern::Apply(Expression& e)
{
    search.ChangeState(*this);
    e.Walk(*this);
    if (state == &found)
    {
        e.Walk(copy);
        return copy.Get();
    }
    return e;
}

bool SumPattern::SearchState::Visit(SumPattern& pattern, Add& add)
{
    pattern.check.ChangeState(pattern);
    return pattern.state->Check(pattern, add);
}


bool SumPattern::SearchState::Visit(SumPattern& pattern, Sum& sum)
{
    pattern.check.ChangeState(pattern);
    return pattern.state->Check(pattern, sum);
}

bool SumPattern::SearchState::Default(SumPattern& pattern, Expression& e)
{
    //keep going down expression
    return true;
}

bool SumPattern::CheckState::Check(SumPattern& pattern, Add& e)
{
    values.clear();
    e.lhs.Walk(pattern);
    e.rhs.Walk(pattern);
    if (values.size() > 2)
    {
        Sum& res = pattern.facade.sumFactory.Create(values);
        pattern.found.res = &res;
        pattern.copy.SetReplacement(e, *pattern.found.res);
        pattern.found.ChangeState(pattern);
        //false stops the search
        return false;
    }
    pattern.search.ChangeState(pattern);
    return true;
}


bool SumPattern::CheckState::Check(SumPattern& pattern, Sum& e)
{
    values.clear();
    e.Walk(pattern);
    if (values.size() > e.values.size())
    {
        Sum& res = pattern.facade.sumFactory.Create(values);
        pattern.found.res = &res;
        pattern.copy.SetReplacement(e, *pattern.found.res);
        pattern.found.ChangeState(pattern);
        //false stops the search
        return false;
    }
    pattern.search.ChangeState(pattern);
    return true;
}

bool SumPattern::CheckState::Visit(SumPattern& pattern, Subtract& e)
{
    //lhs do normal
    e.lhs.Walk(pattern);
    //rhs we need to negate the value
    //save the current values to be added
    std::vector<Expression*> copy(values);
    values.clear();
    e.rhs.Walk(pattern);
    Scalar& neg = pattern.facade.scalarFactory.Create(-1);
    //all values are from the subtraction and need to be negated
    for (int i = 0; i < values.size(); ++i)
    {
        //note this can cause double and triple nagatives, that will be fixed in a different pattern
        copy.push_back(&pattern.facade.multiplyFactory.Create(neg, *values[i]));
    }
    //finally update values
    values = copy;
    return false;
}

bool SumPattern::CheckState::Visit(SumPattern& pattern, Add& e)
{
    return true;
}

bool SumPattern::CheckState::Visit(SumPattern& pattern, Sum& e)
{
    return true;
}

bool SumPattern::CheckState::Default(SumPattern& pattern, Expression& e)
{
    //this expression is not doing addition, but its being added so put it in the expresion list
    values.push_back(&e);
    return false;
}

bool SumPattern::FoundState::Default(SumPattern& pattern, Expression& e)
{
    //stop going down expression
    return false;
}