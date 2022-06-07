#include "pch.h"

using ::testing::An;
using ::testing::Matcher;
using ::testing::TypedEq;
using ::testing::Return;
using ::testing::Sequence;

TEST(VisitorTest, VisitAdd)
{
    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Add&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(2).WillRepeatedly(Return(true));
    Variable var;
    Add add(var, var);
    add.Walk(vistor);
}

TEST(EqualityTest, CheckEquality)
{
    //ax
    Variable a;
    Variable x;
    Multiply ax(a, x);
    Multiply ax2(a, x);
    EXPECT_TRUE(ax == ax2);
}

TEST(CopyTest, CopyFactor)
{
    //ax + bx
    Variable a;
    Variable b;
    Variable x;
    Multiply ax(a, x);
    Multiply bx(b, x);
    Add e(ax, bx);

    FactoryFacade facade;
    Copy cp(facade);
    e.Walk(cp);
    Expression &newE = cp.Get();
    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Add&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(4).WillRepeatedly(Return(true));
    newE.Walk(vistor);
}

TEST(SumTest, makesum)
{
    Scalar a(1);
    Scalar b(2);
    Scalar c(3);
    std::vector<Expression*> v;
    v.push_back(&a);
    v.push_back(&b);
    v.push_back(&c);
    Sum abc(v);
    Graph g;
    std::string s = g.Start(abc);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Sum&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(3).WillRepeatedly(Return(true));
    abc.Walk(vistor);
}

TEST(SumTest, CombineSums)
{
    Scalar a(1);
    Scalar b(2);
    Scalar c(3);
    std::vector<Expression*> v;
    v.push_back(&a);
    v.push_back(&b);
    v.push_back(&c);
    Sum abc(v);
    v.push_back(&abc);
    v.push_back(&abc);
    v.push_back(&abc);
    // a + b + c + (a + b + c) + (a + b + c) + (a + b + c)
    Sum combine(v);
    FactoryFacade f;
    SumPattern sp(f);
    Graph g;
    std::string s = g.Start(combine);
    Expression &res = sp.Apply(combine);
    s = g.Start(res);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Sum&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(12).WillRepeatedly(Return(true));
    res.Walk(vistor);
}

TEST(SumTest, CombineSumsAdd)
{
    Scalar a(1);
    Scalar b(2);
    Scalar c(3);
    std::vector<Expression*> v;
    v.push_back(&a);
    v.push_back(&b);
    v.push_back(&c);
    Sum abc(v);
    // (a + b + c) + (a + b + c)
    Add combine(abc, abc);
    FactoryFacade f;
    SumPattern sp(f);
    Graph g;
    std::string s = g.Start(combine);
    Expression& res = sp.Apply(combine);
    s = g.Start(res);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Sum&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(6).WillRepeatedly(Return(true));
    res.Walk(vistor);
}

TEST(SumTest, CombineAdds)
{
    Scalar a(1);
    Add x(a, a);
    Add y(x, a);
    Add z(y, a);
    //a + (a + (a + (a + a))
    Add combine(z, a);
    FactoryFacade f;
    SumPattern sp(f);
    Graph g;
    std::string s = g.Start(combine);
    Expression& res = sp.Apply(combine);
    s = g.Start(res);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Sum&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(5).WillRepeatedly(Return(true));
    res.Walk(vistor);
}

TEST(SumTest, CombineAddSubtract)
{
    Scalar a(1);
    Add x(a, a);
    Subtract y(a, x);
    Add z(y, a);
    //a + (a + (a - (a + a))
    Add combine(z, a);
    FactoryFacade f;
    SumPattern sp(f);
    Graph g;
    std::string s = g.Start(combine);
    // a + a + a + -1 * a + -1 * a
    Expression& res = sp.Apply(combine);
    s = g.Start(res);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Sum&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(7).WillRepeatedly(Return(true));
    res.Walk(vistor);
}

TEST(ZeroPattern, ZeroTest1)
{
    //(a-a) * a
    Variable a;
    Subtract s(a, a);
    Multiply m(s,a);

    FactoryFacade f;
    ZeroPattern z(f);

    // 0 * a
    Expression &res = z.Apply(m);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(1).WillRepeatedly(Return(true));
    res.Walk(vistor);

    // 0
    Expression& res2 = z.Apply(res);

    Scalar* p = dynamic_cast<Scalar*>(&res2);
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(p->value == 0);
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(1).WillRepeatedly(Return(true));
    res2.Walk(vistor);
}

TEST(ZeroPattern, ZeroTest2)
{
    // (5 +(( 5 - 5) + x)) * (5+((x+x)-(x+x)))
    Variable x;
    Scalar five(5);
    //5-5
    Subtract s(five, five);
    // 5-5+x
    Add a1(s, x);
    //(5 + ((5 - 5) + x))
    Add a2(five, a1);
    //x+x
    Add b(x, x);
    // (x+x)-(x+x)
    Subtract b2(b, b);
    //(5+((x+x)-(x+x)))
    Add b3(five, b2);
    // (5 +(( 5 - 5) + x)) * (5+((x+x)-(x+x)))
    Multiply m(a2, b3);

    FactoryFacade f;
    ZeroPattern z(f);

    // (5 +(0 + x)) * (5+((x+x)-(x+x)))
    Graph g;
    std::string gs = g.Start(m);
    Expression& res = z.Apply(m);
    gs = g.Start(res);

    MockVisitor vistor;
    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(5).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(3).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Subtract&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Add&>())).Times(5).WillRepeatedly(Return(true));
    res.Walk(vistor);

    // (5 + x) * (5+((x+x)-(x+x)))
    Expression& res2 = z.Apply(res);
    gs = g.Start(res2);

    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(5).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Subtract&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Add&>())).Times(4).WillRepeatedly(Return(true));
    res2.Walk(vistor);

    // (5 + x) * (5+(0))
    Expression& res3 = z.Apply(res2);
    gs = g.Start(res3);

    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(3).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Subtract&>())).Times(0).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Add&>())).Times(2).WillRepeatedly(Return(true));
    res3.Walk(vistor);

    // (5 + x) * 5
    Expression& res4 = z.Apply(res3);
    gs = g.Start(res4);

    EXPECT_CALL(vistor, Visit(An<Variable&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Scalar&>())).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Subtract&>())).Times(0).WillRepeatedly(Return(true));
    EXPECT_CALL(vistor, Visit(An<Add&>())).Times(1).WillRepeatedly(Return(true));
    res4.Walk(vistor);
}