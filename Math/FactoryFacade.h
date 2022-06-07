#pragma once

#include "AddFactory.h"
#include "SubtractFactory.h"
#include "MultiplyFactory.h"
#include "DivideFactory.h"
#include "VariableFactory.h"
#include "ScalarFactory.h"
#include "SumFactory.h"
#include "ProductsFactory.h"

class FactoryFacade
{
public:
    AddFactory addFactory;
    SubtractFactory subtractFactory;
    MultiplyFactory multiplyFactory;
    DivideFactory divideFactory;
    VariableFactory variableFactory;
    ScalarFactory scalarFactory;
    SumFactory sumFactory;
    ProductsFactory productsFactory;
};