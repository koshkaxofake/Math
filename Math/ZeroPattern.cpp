#include "ZeroPattern.h"
#include "Add.h"
#include "Multiply.h"

Expression& ZeroPattern::Apply(Expression& e)
{
    search.ChangeState(*this);
    isZero = false;
    e.Walk(*this);
    if (state == &found)
    {
        e.Walk(copy);
        return copy.Get();
    }
    return e;
}
bool ZeroPattern::SearchState::Visit(ZeroPattern& pattern, Subtract& e)
{
    //if the values are the same they become zero
    if (e.lhs == e.rhs)
    {
        Scalar& zero = pattern.facade.scalarFactory.Create(0);
        pattern.found.res = &zero;
        pattern.copy.SetReplacement(e, *pattern.found.res);
        pattern.found.ChangeState(pattern);
        //false stops the search
        return false;
    }
    //subtracting 0 does nothing, so we can remove the operation and leave just lhs
    if (e.rhs == Scalar(0))
    {
        pattern.found.res = &e.lhs;
        pattern.copy.SetReplacement(e, *pattern.found.res);
        pattern.found.ChangeState(pattern);
        //false stops the search
        return false;
    }
    // 0 - e, we need to change the subtraction into a times -1
    if (e.lhs == Scalar(0))
    {
        Scalar& neg = pattern.facade.scalarFactory.Create(-1);
        Multiply& m = pattern.facade.multiplyFactory.Create(neg, e.rhs);
        pattern.found.res = &m;
        pattern.copy.SetReplacement(e, *pattern.found.res);
        pattern.found.ChangeState(pattern);
        //false stops the search
        return false;
    }
    return true;
}

bool ZeroPattern::SearchState::Visit(ZeroPattern& pattern, Add& e)
{
    //adding 0 does nothing, so we can remove the operation and leave just lhs
    if (e.rhs == Scalar(0))
    {
        pattern.found.res = &e.lhs;
        pattern.copy.SetReplacement(e, *pattern.found.res);
        pattern.found.ChangeState(pattern);
        //false stops the search
        return false;
    }
    if (e.lhs == Scalar(0))
    {
        pattern.found.res = &e.rhs;
        pattern.copy.SetReplacement(e, *pattern.found.res);
        pattern.found.ChangeState(pattern);
        //false stops the search
        return false;
    }
    return true;
}

bool ZeroPattern::SearchState::Visit(ZeroPattern& pattern, Multiply& e)
{
    pattern.check.ChangeState(pattern);
    return pattern.state->Check(pattern, e);
}

bool ZeroPattern::SearchState::Default(ZeroPattern& pattern, Expression& e)
{
    //keep going down expression
    return true;
}

bool ZeroPattern::CheckState::Check(ZeroPattern& pattern, Subtract& e)
{
    //adding 0 does nothing, so we can remove the operation and leave just lhs
    if (e.rhs == Scalar(0))
    {
        pattern.found.res = &e.lhs;
        pattern.copy.SetReplacement(e, *pattern.found.res);
        pattern.found.ChangeState(pattern);
        //false stops the search
        return false;
    }
    if (e.lhs == Scalar(0))
    {
        pattern.found.res = &e.rhs;
        pattern.copy.SetReplacement(e, *pattern.found.res);
        pattern.found.ChangeState(pattern);
        //false stops the search
        return false;
    }
    //todo A + -1*A?
    pattern.search.ChangeState(pattern);
    return true;
}


bool ZeroPattern::CheckState::Check(ZeroPattern& pattern, Multiply& e)
{
    e.lhs.Walk(pattern);
    bool lhsZero = pattern.isZero;
    e.rhs.Walk(pattern);
    if (pattern.isZero || lhsZero)
    {
        Scalar& zero = pattern.facade.scalarFactory.Create(0);
        pattern.found.res = &zero;
        pattern.copy.SetReplacement(e, *pattern.found.res);
        pattern.found.ChangeState(pattern);
        //false stops the search
        return false;
    }
    pattern.search.ChangeState(pattern);
    return true;
}

bool ZeroPattern::CheckState::Visit(ZeroPattern& pattern, Scalar& e)
{
    if (e.value == 0)
    {
        pattern.isZero = true;
    }
    return true;
}

bool ZeroPattern::CheckState::Default(ZeroPattern& pattern, Expression& e)
{
    return false;
}

bool ZeroPattern::FoundState::Default(ZeroPattern& pattern, Expression& e)
{
    //stop going down expression
    return false;
}
