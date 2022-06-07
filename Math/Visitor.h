#pragma once

class Expression;
class Add;
class Subtract;
class Multiply;
class Divide;
class Variable;
class Sum;
class Products;
class Scalar;

class Visitor
{
public:
    virtual bool Visit(Expression& e);
    virtual bool Visit(Add& e);
    virtual bool Visit(Subtract& e);
    virtual bool Visit(Multiply& e);
    virtual bool Visit(Divide& e);
    virtual bool Visit(Variable& e);
    virtual bool Visit(Scalar& e);
    virtual bool Visit(Sum& e);
    virtual bool Visit(Products& e);
    virtual bool Default(Expression& e);
    const bool Continue = true;
    const bool Stop = false;
};