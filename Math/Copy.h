#pragma once

#include "Expression.h"
#include "Visitor.h"
#include <vector>
#include "Add.h"
#include "Products.h"
#include "Sum.h"
#include "FactoryFacade.h"

class Copy :public Visitor
{
public:
    Copy(FactoryFacade & facade) :replace(nullptr), replacement(nullptr), done(false), factoryFacade(facade){}
    void SetReplacement(Expression& replace, Expression& replacement);
    Expression& Get();
    Expression* replace;
    Expression* replacement;
private:
    bool CheckReplace(Expression&);
    bool Visit(Add& add);
    bool Visit(Sum& add);
    bool Visit(Subtract& sub);
    bool Visit(Products& mult);
    bool Visit(Multiply& mult);
    bool Visit(Divide& divide);
    bool Visit(Scalar& scalar);
    bool Visit(Variable& var);
    bool Visit(Expression& e);
    Expression *last;
    FactoryFacade &factoryFacade;
    bool done;
};