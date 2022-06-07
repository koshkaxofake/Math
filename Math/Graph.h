#pragma once

#include "Visitor.h"
#include "string"
#include "Products.h"
#include "Sum.h"
#include <unordered_map>

class Namer :public Visitor
{
public:
    void Start(Expression& expression);
    bool Visit(Add& add);
    bool Visit(Sum& add);
    bool Visit(Products& mult);
    bool Visit(Multiply& mult);
    bool Visit(Subtract& e);
    bool Visit(Divide& e);
    bool Visit(Variable& e);
    bool Visit(Scalar& e);
    bool Visit(Expression& e);
    std::unordered_map<Expression*, std::string> names;
    int counter;
};

class Graph :public Visitor
{
public:
    std::string Start(Expression& expression);
    bool Visit(Add& add);
    bool Visit(Multiply& mult);
    bool Visit(Sum& add);
    bool Visit(Products& mult);
    bool Visit(Subtract& e);
    bool Visit(Divide& e);
    bool Visit(Variable& e);
    bool Visit(Scalar& e);
    bool Visit(Expression& e);
    std::string output;
    std::string shapes;
    int arrowCounter;
    Namer namer;
};
