#pragma once

#include  "Pattern.h"
#include "Factor.h"
#include "Copy.h"

class SumPattern : public Pattern
{
public:
    SumPattern(FactoryFacade& facade) : state(&search), facade(facade), copy(facade) {}
    Expression& Apply(Expression& e);
    bool Visit(Add& add)
    {
        return state->Visit(*this, add);
    }
    bool Visit(Sum& sum)
    {
        return state->Visit(*this, sum);
    }
    bool Visit(Subtract& sub)
    {
        return state->Visit(*this, sub);
    }
    bool Default(Expression& e)
    {
        return state->Default(*this, e);
    }
private:
    class State
    {
    public:
        virtual void ChangeState(SumPattern& pattern)
        {
            pattern.state = this;
        }
        virtual bool Visit(SumPattern& pattern, Add& add)
        {
            return Default(pattern, add);
        }
        virtual bool Visit(SumPattern& pattern, Sum& sum)
        {
            return Default(pattern, sum);
        }
        virtual bool Visit(SumPattern& pattern, Subtract& sub)
        {
            return Default(pattern, sub);
        }
        virtual bool Default(SumPattern& pattern, Expression& e) = 0;
        //checks for the pattern
        virtual bool Check(SumPattern& pattern, Add& e) { return true; }
        virtual bool Check(SumPattern& pattern, Sum& e) { return true; }
    };
    class SearchState : public State
    {
    public:
        bool Visit(SumPattern& pattern, Add& add);
        bool Visit(SumPattern& pattern, Sum& add);
        bool Default(SumPattern& pattern, Expression& e);
    };
    class CheckState : public State
    {
    public:
        void ChangeState(SumPattern& pattern)
        {
            pattern.state = this;
            values.clear();
        }
        bool Check(SumPattern& pattern, Add& e);
        bool Check(SumPattern& pattern, Sum& e);
        bool Visit(SumPattern& pattern, Add& sub);
        bool Visit(SumPattern& pattern, Sum& sub);
        bool Visit(SumPattern& pattern, Subtract& sub);
        bool Default(SumPattern& pattern, Expression& e);
        std::vector<Expression*> values;
    };
    class FoundState : public State
    {
    public:
        bool Default(SumPattern& pattern, Expression& e);
        Expression* res;
    };
    SearchState search;
    CheckState check;
    FoundState found;
    State* state;

    FactoryFacade& facade;
    Copy copy;
};