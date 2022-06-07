#include "pch.h"

using ::testing::An;
using ::testing::Matcher;
using ::testing::TypedEq;
using ::testing::Return;
using ::testing::Sequence;
using ::testing::Ref;

TEST(ScalarTests, RationalAdd)
{
    Rational r1(12345, 542);
    Rational r2(4839, 935);
    Rational sum = r1 + r2;
    Rational res(14165313, 506770);
    EXPECT_TRUE(sum == res);
}

TEST(ScalarTests, RationalSubtract)
{
    Rational r1(12345, 542);
    Rational r2(4839, 935);
    Rational sub = r1 - r2;
    Rational res(8919837, 506770);
    EXPECT_TRUE(sub == res);
}

TEST(ScalarTests, RationalMultiply)
{
    Rational r1(12345, 542);
    Rational r2(4839, 935);
    Rational mult = r1 * r2;
    Rational res(11947491, 101354);
    EXPECT_TRUE(mult == res);
}

TEST(ScalarTests, RationalDivide)
{
    Rational r1(12345, 542);
    Rational r2(4839, 935);
    Rational div = r1 / r2;
    Rational res(3847525, 874246);
    EXPECT_TRUE(div == res);
}

TEST(ScalarTests, BigAssNumber)
{
    Rational r1(9784748398274ll, 54232ll);
    Rational r2(347482487484ll, 932325ll);
    Rational mult = r1 + r2;
    Rational res(1523568370113673223ll, 8426974900ll);
    EXPECT_TRUE(mult == res);
}

TEST(ScalarTests, SimpleScalar)
{
    Scalar five(5);
    Scalar four(4);
    // 5+4
    Add a(five, four);
    // (5+4)*(5+4)
    Multiply m(a, a);

    MockVisitor visitor;
    EXPECT_CALL(visitor, Visit(Matcher<Scalar&>(Ref(five)))).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(visitor, Visit(Matcher<Scalar&>(Ref(four)))).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(visitor, Visit(An<Add&>())).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(visitor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    m.Walk(visitor);
}

TEST(ScalarTests, ScalarArithmetic)
{
    Scalar five(5);
    Scalar four(4);
    // 5+4
    Add a(five, four);
    // (5+4)*(5+4)
    Multiply m(a, a);
    FactoryFacade f;
    ScalarMath sm(f);
    Expression &res = sm.Apply(m);

    Scalar* finalValue = dynamic_cast<Scalar*>(&res);
    ASSERT_NE(finalValue, nullptr);
    EXPECT_EQ(finalValue->value, 81);
}


TEST(ScalarTests, ScalarArithmeticVariable)
{
    Scalar five(5);
    Scalar four(4);
    // 5+4
    Add a(five, four);
    Variable x;
    // (5+4)*x
    Multiply m(a, x);
    FactoryFacade f;
    ScalarMath sm(f);
    Graph g;
    std::string s = g.Start(m);
    Expression& res = sm.Apply(m);
    s = g.Start(res);

    Multiply* mult = dynamic_cast<Multiply*>(&res);
    ASSERT_NE(mult, nullptr);
    Scalar* scalar = dynamic_cast<Scalar*>(&mult->lhs);
    ASSERT_NE(scalar, nullptr);
    EXPECT_EQ(scalar->value, 9);
}

TEST(ScalarTests, ScalarSum)
{
    Scalar five(5);
    Scalar four(4);
    Scalar Three(3);
    // 5+4+3
    std::vector<Expression*> args;
    args.push_back(&five);
    args.push_back(&four);
    args.push_back(&Three);
    Sum sum(args);
    FactoryFacade f;
    ScalarMath sm(f);
    Graph g;
    std::string s = g.Start(sum);
    Expression& res = sm.Apply(sum);
    s = g.Start(res);

    Scalar* finalValue = dynamic_cast<Scalar*>(&res);
    ASSERT_NE(finalValue, nullptr);
    EXPECT_EQ(finalValue->value, 12);
}

TEST(ScalarTests, SumWithVar)
{
    Scalar five(5);
    Scalar four(4);
    Scalar Three(3);
    Variable x;
    // 5+4+3
    std::vector<Expression*> args;
    args.push_back(&five);
    args.push_back(&four);
    args.push_back(&Three);
    args.push_back(&x);
    Sum sum(args);
    FactoryFacade f;
    ScalarMath sm(f);
    Graph g;
    std::string s = g.Start(sum);
    Expression& res = sm.Apply(sum);
    s = g.Start(res);

    Sum* finalValue = dynamic_cast<Sum*>(&res);
    ASSERT_NE(finalValue, nullptr);
    Scalar* combined = dynamic_cast<Scalar*>(finalValue->values[0]);
    ASSERT_NE(combined, nullptr);
    EXPECT_EQ(combined->value, 12);
}
