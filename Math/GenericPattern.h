#pragma once

#include  "Pattern.h"
#include "Factor.h"
#include "Copy.h"
#include <unordered_map>

class GenericPattern : public Pattern
{
public:
    GenericPattern(FactoryFacade& facade, Expression& matcher, Expression &result) : facade(facade), copy(facade), matcher(matcher), result(result) {} 
    Expression& Apply(Expression& e);                                                                                                                                                                                                                                          
    bool Default(Expression& e);                                                                                                                       
private:                                                                                                                                               
    //checks if two things being added are but multiples                                                                                               
    class Checker : public Visitor                                                                                                                     
    {                                                                                                                                                  
    public:    
        Checker(Expression& matcher) : matcher(matcher)
        {

        }
        bool Check(Expression& e);
        bool Visit(Add& e);
        bool Visit(Subtract& e);
        bool Visit(Multiply& e);
        bool Visit(Divide& e);
        bool Visit(Variable& e);
        bool Visit(Scalar& e);
        bool Visit(Sum& e);
        bool Visit(Products& e);
        bool Default(Expression& e);
        //current is the node we are on in the matcher expression
        Expression* current;
        std::unordered_map<Expression*, Expression*> map;
        Expression& matcher;
        bool failed;
    };
    class Creator : public Visitor
    {
    public:
        Creator(std::unordered_map<Expression*, Expression*> &map, FactoryFacade &facade) : map(map), facade(facade)
        {
            returnValue = nullptr;
        }
        bool Visit(Add& e);
        bool Visit(Subtract& e);
        bool Visit(Multiply& e);
        bool Visit(Divide& e);
        bool Visit(Variable& e);
        bool Visit(Scalar& e);
        bool Visit(Sum& e);
        bool Visit(Products& e);
        bool Default(Expression& e);
        //the expression that was made in the previous visit call
        Expression* returnValue;
        std::unordered_map<Expression*, Expression*> &map;
        FactoryFacade& facade;
    };

    bool found;
    FactoryFacade& facade;
    Expression& matcher;
    Expression& result;
    Factor factor;
    Copy copy;
};