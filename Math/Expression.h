#pragma once

class Visitor;

class Expression
{
public:
    ~Expression() {}
    virtual void Walk(Visitor& v) = 0;
    virtual bool operator==(const Expression& other) const
    {
        return &other == this;
    }
    bool operator!=(const Expression& other) const
    {
        return !(other == *this);
    }
};