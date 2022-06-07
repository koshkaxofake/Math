#include "PatternStub.h"
#include "Add.h"
#include "Multiply.h"

Expression& PatternStub::Apply(Expression& e)
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

bool PatternStub::SearchState::Default(PatternStub& pattern, Expression& e)
{
    //keep going down expression
    return true;
}

bool PatternStub::CheckState::Default(PatternStub& pattern, Expression& e)
{
    return false;
}

bool PatternStub::FoundState::Default(PatternStub& pattern, Expression& e)
{
    //stop going down expression
    return false;
}