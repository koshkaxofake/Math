#include "Visitor.h"
#include "Add.h"
#include "Subtract.h"
#include "Multiply.h"
#include "Divide.h"
#include "Variable.h"
#include "Scalar.h"
#include "Sum.h"
#include "Products.h"

bool Visitor::Visit(Expression& e) { return Default(e); }
bool Visitor::Visit(Add& e) { return Default(e); }
bool Visitor::Visit(Subtract& e) { return Default(e); }
bool Visitor::Visit(Multiply& e) { return Default(e); }
bool Visitor::Visit(Divide& e) { return Default(e); }
bool Visitor::Visit(Variable& e) { return Default(e); }
bool Visitor::Visit(Scalar& e) { return Default(e); }
bool Visitor::Visit(Sum& e) { return Default(e); }
bool Visitor::Visit(Products& e) { return Default(e); }
//if the inherited visitor does not override a specific visit method this will be called
bool Visitor::Default(Expression& e) { return true; }