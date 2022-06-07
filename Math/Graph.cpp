
#include "Graph.h"
#include "Add.h"
#include "Multiply.h"
#include "Subtract.h"
#include "Divide.h"
#include "Variable.h"
#include "Scalar.h"
#include "Expression.h"
#include "Rational.h"
#include <sstream>

void Namer::Start(Expression& expression)
{
    names.clear();
    counter = 0;
    expression.Walk(*this);
}

bool Namer::Visit(Add& e)
{
    std::stringstream ss;
    ss << "Add" << counter++;
    names[&e] = ss.str();
    return true;
}
bool Namer::Visit(Sum& e)
{
    std::stringstream ss;
    ss << "Sum" << counter++;
    names[&e] = ss.str();
    return true;
}
bool Namer::Visit(Products& e)
{
    std::stringstream ss;
    ss << "Products" << counter++;
    names[&e] = ss.str();
    return true;
}
bool Namer::Visit(Multiply& e)
{
    std::stringstream ss;
    ss << "Multiply" << counter++;
    names[&e] = ss.str();
    return true;
}
bool Namer::Visit(Subtract& e)
{
    std::stringstream ss;
    ss << "Subtract" << counter++;
    names[&e] = ss.str();
    return true;
}
bool Namer::Visit(Divide& e)
{
    std::stringstream ss;
    ss << "Divide" << counter++;
    names[&e] = ss.str();
    return true;
}
bool Namer::Visit(Variable& e)
{
    std::stringstream ss;
    ss << "Variable" << counter++;
    names[&e] = ss.str();
    return true;
}
bool Namer::Visit(Scalar& e)
{
    std::stringstream ss;
    ss << "Scalar" << counter++ << "v" << e.value.getValue();
    names[&e] = ss.str();
    return true;
}
bool Namer::Visit(Expression& e)
{
    std::stringstream ss;
    ss << "Expression" << counter++;
    names[&e] = ss.str();
    return true;
}

std::string Graph::Start(Expression &expression)
{
    namer.Start(expression);
    arrowCounter = 0;
    output = "";
    shapes = "digraph G {{";
    expression.Walk(*this);
    shapes += "}";
    output = shapes + output + "}";
    return output;
}

bool Graph::Visit(Sum& e)
{
    std::stringstream ss;
    ss << namer.names[&e] << " [shape=square]\n";
    shapes += ss.str();
    ss.str("");
    for (int i = 0; i < e.values.size(); ++i)
    {
        ss << namer.names[&e] << " -> " << namer.names[e.values[i]] << "[label=" << arrowCounter << "]\n";
    }
    arrowCounter += 1;
    output += ss.str();
    return true;
}

bool Graph::Visit(Add& e)
{
    std::stringstream ss;
    ss << namer.names[&e] << " [shape=square]\n";
    shapes += ss.str();
    ss.str("");
    ss << namer.names[&e] << " -> " << namer.names[&e.lhs] << "[label=" << arrowCounter << "]\n";
    ss << namer.names[&e] << " -> " << namer.names[&e.rhs] << "[label=" << arrowCounter << "]\n";
    arrowCounter += 1;
    output += ss.str();
    return true;
}
bool Graph::Visit(Products& e)
{
    std::stringstream ss;
    ss << namer.names[&e] << " [shape=square]\n";
    shapes += ss.str();
    ss.str("");
    for (int i = 0; i < e.values.size(); ++i)
    {
        ss << namer.names[&e] << " -> " << namer.names[e.values[i]] << "[label=" << arrowCounter << "]\n";
    }
    arrowCounter += 1;
    output += ss.str();
    return true;
}
bool Graph::Visit(Multiply& e)
{
    std::stringstream ss;
    ss << namer.names[&e] << " [shape=square]\n";
    shapes += ss.str();
    ss.str("");
    ss << namer.names[&e] << " -> " << namer.names[&e.lhs] << "[label=" << arrowCounter << "]\n";
    ss << namer.names[&e] << " -> " << namer.names[&e.rhs] << "[label=" << arrowCounter << "]\n";
    arrowCounter += 1;
    output += ss.str();
    return true;
}
bool Graph::Visit(Subtract& e)
{
    std::stringstream ss;
    ss << namer.names[&e] << " [shape=square]\n";
    shapes += ss.str();
    ss.str("");
    ss << namer.names[&e] << " -> " << namer.names[&e.lhs] << "[label=" << arrowCounter << "]\n";
    ss << namer.names[&e] << " -> " << namer.names[&e.rhs] << "[label=" << arrowCounter << "]\n";
    arrowCounter += 1;
    output += ss.str();
    return true;
}
bool Graph::Visit(Divide& e)
{
    std::stringstream ss;
    ss << namer.names[&e] << " [shape=square]\n";
    shapes += ss.str();
    ss.str("");
    ss << namer.names[&e] << " -> " << namer.names[&e.lhs] << "[label=" << arrowCounter << "]\n";
    ss << namer.names[&e] << " -> " << namer.names[&e.rhs] << "[label=" << arrowCounter << "]\n";
    arrowCounter += 1;
    output += ss.str();
    return true;
}
bool Graph::Visit(Variable& e)
{
    std::stringstream ss;
    ss << namer.names[&e] << " [shape=square]\n";
    shapes += ss.str();
    return true;
}
bool Graph::Visit(Scalar& e)
{
    std::stringstream ss;
    ss << namer.names[&e] << " [shape=square]\n";
    shapes += ss.str();
    return true;
}
bool Graph::Visit(Expression& e)
{
    std::stringstream ss;
    ss << namer.names[&e] << " [shape=square]\n";
    shapes += ss.str();
    return true;
}