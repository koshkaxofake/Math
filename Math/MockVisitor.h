#pragma once
#include "Visitor.h"
#include "gmock/gmock.h"

class MockVisitor : public Visitor
{
public:
    MOCK_METHOD1(Visit, bool(Expression&));
    MOCK_METHOD1(Visit, bool(Multiply&));
    MOCK_METHOD1(Visit, bool(Add&));
    MOCK_METHOD1(Visit, bool(Sum&));
    MOCK_METHOD1(Visit, bool(Products&));
    MOCK_METHOD1(Visit, bool(Subtract&));
    MOCK_METHOD1(Visit, bool(Divide&));
    MOCK_METHOD1(Visit, bool(Scalar&));
    MOCK_METHOD1(Visit, bool(Variable&));
};