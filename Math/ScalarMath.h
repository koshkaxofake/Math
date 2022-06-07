#pragma once

#include  "Pattern.h"
#include "Factor.h"
#include "Copy.h"
#include "Rational.h"

class ScalarMath: public Pattern
{
public:
    ScalarMath(FactoryFacade& facade) :copy(facade), facade(facade), replace(nullptr), allScalar(false){}
    Expression& Apply(Expression& e);
    bool Visit(Add&);
    bool Visit(Sum&);
    bool Visit(Multiply&);
    bool Visit(Divide& );
    bool Visit(Subtract&);
    bool Visit(Scalar&);
    bool Default(Expression& e);
private:
    Copy copy;
    FactoryFacade facade;
    Expression* replace;
    Expression* replacement;
    Rational value;
    bool allScalar;
};