#include "pch.h"

using ::testing::An;
using ::testing::Matcher;
using ::testing::TypedEq;
using ::testing::Return;
using ::testing::Sequence;
using ::testing::Ref;

TEST(Parse, SimpleParse)
{
    std::string equation = "x+x*x";
    FactoryFacade facade;
    Parser parser(facade);
    std::vector<std::string> res = parser.ParseString(equation);
    EXPECT_EQ(res.size(), 5);
}

TEST(Parse, ExpressionParse)
{
    std::string equation = "x+x*x";
    FactoryFacade facade;
    Parser parser(facade);
    Expression& res = parser.Parse(equation);


    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(3).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Add&>())).Times(1).WillRepeatedly(Return(true));
    res.Walk(vistor);
}

TEST(Parse, ExpressionParseOrder)
{
    std::string equation = "x+x*x";
    FactoryFacade facade;
    Parser parser(facade);
    Expression& res = parser.Parse(equation);

    //check that the first expression is multiply not add
    EXPECT_NE(dynamic_cast<Multiply*>(&res), nullptr);
}

TEST(Parse, NegativeSign)
{
    std::string equation = "x+-x";
    FactoryFacade facade;
    Parser parser(facade);
    Expression& res = parser.Parse(equation);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(3).WillRepeatedly(Return(true));
    //-x becomes -1*x which is a multiply and a scalar
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Add&>())).Times(1).WillRepeatedly(Return(true));
    res.Walk(vistor);
}

//this test wil test the order of operations is correct
TEST(Parse, ArithmeticOrder)
{
    std::string equation = "6/2(1+2)";//9
    FactoryFacade facade;
    Parser parser(facade);
    Expression& eq = parser.Parse(equation);
   
    ScalarMath sm(facade);
    Expression& res = sm.Apply(eq);

    Scalar* scalar = dynamic_cast<Scalar*>(&res);
    ASSERT_NE(scalar, nullptr);
    EXPECT_EQ(scalar->value, 9);
}
