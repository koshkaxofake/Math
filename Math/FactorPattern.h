#pragma once

#include  "Pattern.h"
#include "Factor.h"
#include "Copy.h"
#include <unordered_map>

class FactorPattern : public Pattern
{
public:
    FactorPattern(FactoryFacade &facade) : facade(facade), copy(facade) {}
    Expression &Apply(Expression &e);
    bool Visit(Add& add);
    bool Visit(Sum& sum);
private:
    //checks if two things being added are but multiples
    class FactorChecker : public Visitor
    {
    public:
        bool Check(Add& e);
        bool Visit(Multiply& mult);
        bool Default(Expression& e);
        Multiply* lhs;
        Multiply* rhs;
    };
    //checks for factors withen sum
    class FactorCheckerSum : public Visitor
    {
    public:
        FactorCheckerSum(FactoryFacade &facade) : facade(facade)
        {}
        bool Check(Sum& e);
        bool Visit(Multiply& mult);
        bool Default(Expression& e);
        Expression* result;
        std::vector<Multiply*> values;
        FactoryFacade& facade;
    };

    bool found;
    FactoryFacade& facade;
    Factor factor;
    Copy copy;
};