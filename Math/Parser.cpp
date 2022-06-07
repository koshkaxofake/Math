#include "Parser.h"

Parser::Parser(FactoryFacade &facade) :facade(facade)
{

}

Expression& Parser::Parse(std::string equation)
{
    return facade.variableFactory.Create();
}

std::vector<std::string> Parser::ParseString(std::string equation)
{
    return std::vector<std::string>();
}