
#include "GenricPatternFacade.h"

GenericPatternFacade::GenericPatternFacade(FactoryFacade &facade)
{
    // A + A  -> 2 * A
    Variable& a = facade.variableFactory.Create();
    Add &matcher = facade.addFactory.Create(a, a);

    //2 * a
    Scalar &two = facade.scalarFactory.Create(2);
    Multiply &result = facade.multiplyFactory.Create(two, a);

    patterns["AddTwoSame"] = new GenericPattern(facade, matcher, result);
}
GenericPatternFacade::~GenericPatternFacade()
{
    for (auto it = patterns.begin(); it != patterns.end(); ++it)
        delete it->second;
}
GenericPattern& GenericPatternFacade::GetPattern(std::string patternName)
{
    return *patterns[patternName];
}