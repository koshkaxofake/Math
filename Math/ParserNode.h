#pragma once
#include <unordered_map>

class ParserNode
{
public:
    std::unordered_map<char, ParserNode*> links;
};