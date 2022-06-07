#pragma once

#include  "Pattern.h"
#include "Factor.h"
#include "Copy.h"
#include "Rational.h"

//pattern will replace expressions that evaluate to zero and remove them from the main expression
class ZeroPattern : public Pattern
{
public:
    ZeroPattern(FactoryFacade& facade) : state(&search), copy(facade), facade(facade), replace(nullptr), isZero(false) {}
    Expression& Apply(Expression& e);


    bool Visit(Multiply& e)
    {
        return state->Visit(*this, e);
    }
    bool Visit(Scalar& e)
    {
        return state->Visit(*this, e);
    }
    bool Visit(Add& e)
    {
        return state->Visit(*this, e);
    }
    bool Visit(Subtract& e)
    {
        return state->Visit(*this, e);
    }
    bool Default(Expression& e)
    {
        return state->Default(*this, e);
    }
private:
    class State
    {
    public:
        virtual void ChangeState(ZeroPattern& pattern)
        {
            pattern.state = this;
        }
        virtual bool Visit(ZeroPattern& pattern, Add& add)
        {
            return Default(pattern, add);
        }
        virtual bool Visit(ZeroPattern& pattern, Multiply& e)
        {
            return Default(pattern, e);
        }
        virtual bool Visit(ZeroPattern& pattern, Subtract& sub)
        {
            return Default(pattern, sub);
        }
        virtual bool Visit(ZeroPattern& pattern, Scalar& e)
        {
            return Default(pattern, e);
        }
        virtual bool Default(ZeroPattern& pattern, Expression& e) = 0;
        //checks for the pattern
        virtual bool Check(ZeroPattern& pattern, Add& e) { return true; }
        virtual bool Check(ZeroPattern& pattern, Subtract& e) { return true; }
        virtual bool Check(ZeroPattern& pattern, Multiply& e) { return true; }
        virtual bool Check(ZeroPattern& pattern, Sum& e) { return true; }
    };
    class SearchState : public State
    {
    public:
        bool Visit(ZeroPattern& pattern, Subtract& e);
        bool Visit(ZeroPattern& pattern, Add& e);
        bool Visit(ZeroPattern& pattern, Multiply& e);
        bool Default(ZeroPattern& pattern, Expression& e);
    };
    class CheckState : public State
    {
    public:
        void ChangeState(ZeroPattern& pattern)
        {
            pattern.state = this;
            values.clear();
        }
        bool Check(ZeroPattern& pattern, Subtract& e);
        bool Check(ZeroPattern& pattern, Multiply& e);
        bool Visit(ZeroPattern& pattern, Scalar& e);
        bool Default(ZeroPattern& pattern, Expression& e);
        std::vector<Expression*> values;
    };
    //state to check for x + -x
    class CheckAddState : public State
    {
    public:
        void ChangeState(ZeroPattern& pattern)
        {
            pattern.state = this;
            lhsNegative = false;
            rhsNegative = false;
            lhs = nullptr;
            rhs = nullptr;
        }
        bool Check(ZeroPattern& pattern, Add& e);
        bool Visit(ZeroPattern& pattern, Multiply& e);
        bool Default(ZeroPattern& pattern, Expression& e);
        bool lhsNegative;
        bool rhsNegative;
        Expression *lhs;
        Expression *rhs;
    };
    class FoundState : public State
    {
    public:
        bool Default(ZeroPattern& pattern, Expression& e);
        Expression* res;
    };
    SearchState search;
    CheckState check;
    FoundState found;
    State* state;


    Copy copy;
    FactoryFacade facade;
    Expression* replace;
    Expression* replacement;
    Rational value;
    bool isZero;
};