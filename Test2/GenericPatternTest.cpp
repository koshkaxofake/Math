#include "pch.h"

using ::testing::An;
using ::testing::Matcher;
using ::testing::TypedEq;
using ::testing::Ref;
using ::testing::Return;


TEST(GenericPatternTest, GenericFactor)
{
    //ax + bx
    Variable a;
    Variable b;
    Variable x;
    Multiply ax(a, x);
    Multiply bx(b, x);
    Add matcher(ax, bx);

    //x * (a+b)
    Add ab(a,b);
    Multiply result(x, ab);

    //test expression my + ny
    Variable m;
    Variable n;
    Variable y;
    Multiply my(m, y);
    Multiply ny(n, y);
    Add test(my, ny);

    FactoryFacade facade;
    GenericPattern p(facade, matcher, result);
    // y * (m + n)
    Expression &res = p.Apply(test);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Add&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(3).WillRepeatedly(Return(true));
    res.Walk(vistor);
}

TEST(GenericPatternTest, AddToMultiply)
{
    //a + a
    Variable a;
    Add matcher(a, a);

    //2 * a
    Scalar two(2);
    Multiply result(two, a);

    //test expression m + m
    Variable m;
    Add test(m, m);

    FactoryFacade facade;
    GenericPattern p(facade, matcher, result);
    // 2 * m
    Expression& res = p.Apply(test);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(1).WillRepeatedly(Return(true));
    res.Walk(vistor);
}
TEST(GenericPatternTest, AddToMultiplyFacade)
{
    FactoryFacade facade;
    GenericPatternFacade patternFacade(facade);

    //test expression m + m
    Variable m;
    Add test(m, m);

    // 2 * m
    Expression& res = patternFacade.GetPattern("AddTwoSame").Apply(test);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(1).WillRepeatedly(Return(true));
    res.Walk(vistor);
}


TEST(GenericPatternTest, AddToMultiplyExpression)
{
    //a + a
    Variable a;
    Add matcher(a, a);

    //2 * a
    Scalar two(2);
    Multiply result(two, a);

    //test expression m/m + m/m
    Variable m;
    Divide e(m,m);
    Add test(e, e);

    FactoryFacade facade;
    GenericPattern p(facade, matcher, result);
    // 2 * m/m
    Expression& res = p.Apply(test);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Divide&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(2).WillRepeatedly(Return(true));
    res.Walk(vistor);
}