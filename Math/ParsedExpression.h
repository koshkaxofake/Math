#pragma once

#include "ParseExpression.h"
#include "Expression.h"

class ParsedExpression : public ParseExpression
{
    Expression& expression;
};