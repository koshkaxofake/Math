#pragma once

#pragma once

#pragma once

#include  "Pattern.h"
#include "Factor.h"
#include "Copy.h"
#include "Rational.h"

//This is a class thta can be copied when making new patterns
class PatternStub : public Pattern
{
public:
    PatternStub(FactoryFacade& facade) : state(&search), copy(facade), facade(facade), replace(nullptr) {}
    Expression& Apply(Expression& e);

    template <typename T>
    bool Visit(T& t)
    {
        return state->Visit(*this, t);
    }
    bool Default(Expression& e)
    {
        return state->Default(*this, e);
    }
private:
    class State
    {
    public:
        virtual void ChangeState(PatternStub& pattern)
        {
            pattern.state = this;
        }
        virtual bool Visit(PatternStub& pattern, Add& add)
        {
            return Default(pattern, add);
        }
        virtual bool Visit(PatternStub& pattern, Multiply& sum)
        {
            return Default(pattern, sum);
        }
        virtual bool Visit(PatternStub& pattern, Subtract& sub)
        {
            return Default(pattern, sub);
        }
        virtual bool Visit(PatternStub& pattern, Scalar& sub)
        {
            return Default(pattern, sub);
        }
        virtual bool Visit(PatternStub& pattern, Variable& sub)
        {
            return Default(pattern, sub);
        }
        virtual bool Visit(PatternStub& pattern, Divide& sub)
        {
            return Default(pattern, sub);
        }
        virtual bool Visit(PatternStub& pattern, Sum& sub)
        {
            return Default(pattern, sub);
        }
        virtual bool Visit(PatternStub& pattern, Products& sub)
        {
            return Default(pattern, sub);
        }
        virtual bool Default(PatternStub& pattern, Expression& e) = 0;
        //checks for the pattern
        virtual bool Check(PatternStub& pattern, Expression& e) { return true; }
    };
    class SearchState : public State
    {
    public:
        bool Default(PatternStub& pattern, Expression& e);
    };
    class CheckState : public State
    {
    public:
        void ChangeState(PatternStub& pattern)
        {
            pattern.state = this;
        }
        bool Default(PatternStub& pattern, Expression& e);
    };
    class FoundState : public State
    {
    public:
        bool Default(PatternStub& pattern, Expression& e);
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
};