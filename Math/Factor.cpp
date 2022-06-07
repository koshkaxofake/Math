
#include "Factor.h"
#include "Add.h"

Expression& Factor::Apply(Multiply& mult, Multiply& mult2)
{
    //ac + bx  -> (a+b)*x
    Expression* x =&mult.lhs;
    Expression* a =&mult.lhs;
    Expression* b =&mult.lhs;
    if (mult.lhs == mult2.rhs)
    {
        x = &mult.lhs;
        a = &mult.rhs;
        b = &mult2.lhs;
    }
    if (mult.lhs == mult2.lhs)
    {
        x = &mult.lhs;
        a = &mult.rhs;
        b = &mult2.rhs;
    }
    if (mult.rhs == mult2.rhs)
    {
        x = &mult.rhs;
        a = &mult.lhs;
        b = &mult2.lhs;
    }
    if (mult.rhs == mult2.lhs)
    {
        x = &mult.rhs;
        a = &mult.lhs;
        b = &mult2.rhs;
    }

    adds.emplace_front(*a, *b);
    mults.emplace_front(adds.front(), *x);

    return mults.front();
}