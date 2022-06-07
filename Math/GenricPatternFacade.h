#pragma once


#include "GenericPattern.h"

class GenericPatternFacade
{
public:
    GenericPatternFacade(FactoryFacade& facade);
    ~GenericPatternFacade();
    GenericPattern& GetPattern(std::string patternName);
    std::unordered_map<std::string, GenericPattern*> patterns;
};