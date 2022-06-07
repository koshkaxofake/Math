#pragma once

#include <vector>

class ParseExpression
{
public:
    virtual void Parse(std::vector<ParseExpression*> expressions) = 0;
};