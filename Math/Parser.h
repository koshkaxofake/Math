#pragma once

#include "ParserNode.h"
#include "Expression.h"
#include "FactoryFacade.h"

class Parser
{
public:
    Parser(FactoryFacade &facade);
    ParserNode ParseTree;
    Expression& Parse(std::string equation);
    std::vector<std::string> ParseString(std::string equation);

private:
    FactoryFacade& facade;
};