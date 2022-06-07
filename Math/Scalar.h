#pragma once

#include "Expression.h"
#include "Visitor.h"
#include "Rational.h"

class Scalar : public Expression
{
public:
    Scalar(Rational value) :value(value) {}
    bool operator==(const Expression& other) const
    {
        if (this == &other)
            return true;
        const Scalar* otheScalar = dynamic_cast<const Scalar*>(&other);
        if (!otheScalar)
            return false;
        return value == otheScalar->value;
    }
    virtual void Walk(Visitor& v)
    {
        v.Visit(*this);
    }
    Rational value;
};