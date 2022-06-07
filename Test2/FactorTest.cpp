#include "pch.h"

using ::testing::An;
using ::testing::Matcher;
using ::testing::TypedEq;
using ::testing::Ref;
using ::testing::Return;

class FactorVisitor : public Visitor
{
public:
    MOCK_METHOD1(Visit, bool(Expression&));
    MOCK_METHOD1(Visit, bool(Multiply&));
    MOCK_METHOD1(Visit, bool(Add&));
    MOCK_METHOD1(Visit, bool(Subtract&));
    MOCK_METHOD1(Visit, bool(Divide&));
    MOCK_METHOD1(Visit, bool(Variable&));
};

TEST(FactorTest, factor)
{
    Factor f;
    //create ax + bx
    Variable a;
    Variable b;
    Variable x;
    Multiply ax(a, x);
    Multiply bx(b,x);
    //res = (a+b)*x
    Expression &res = f.Apply(ax, bx);
    
    FactorVisitor v;
    ::testing::Sequence sq;
    EXPECT_CALL(v, Visit(An<Multiply&>())).InSequence(sq).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Add&>())).InSequence(sq).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(a)))).InSequence(sq).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(b)))).InSequence(sq).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(x)))).InSequence(sq).WillRepeatedly(Return(true));
    
    res.Walk(v);
}

TEST(FactorTest, FactorPattern)
{
    FactoryFacade facade;
    FactorPattern f(facade);
    //create ax + bx
    Variable a;
    Variable b;
    Variable x;
    Multiply ax(a, x);
    Multiply bx(b, x);
    Add ex(ax, bx);
    //res = (a+b)*x
    Expression& res = f.Apply(ex);

    MockVisitor v;
    ::testing::Sequence sq;
    EXPECT_CALL(v, Visit(An<Multiply&>())).InSequence(sq).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Add&>())).InSequence(sq).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(a)))).InSequence(sq).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(b)))).InSequence(sq).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(x)))).InSequence(sq).WillRepeatedly(Return(true));

    res.Walk(v);
}

TEST(FactorTest, FactorPatternTwo)
{
    FactoryFacade facade;
    FactorPattern f(facade);
    //create ax + bx
    Variable a;
    Variable b;
    Variable x;
    Multiply ax(a, x);
    Multiply bx(b, x);
    Add axbx(ax, bx);
    //(ax + bx) + (ax + bx)
    Add ex(axbx, axbx);
    //res = (a+b)*x + ax + bx
    Expression& res = f.Apply(ex);

    MockVisitor v;
    EXPECT_CALL(v, Visit(An<Multiply&>())).Times(3).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Add&>())).Times(3).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(a)))).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(b)))).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(x)))).Times(3).WillRepeatedly(Return(true));

    res.Walk(v);
}

TEST(FactorTest, FactorPatternRepeat)
{
    FactoryFacade facade;
    FactorPattern f(facade);
    //create ax + bx
    Variable a;
    Variable c;
    Variable b;
    Variable x;
    Multiply ax(a, x);
    Multiply cx(c, x);
    Multiply bx(b, x);
    Add axbx(ax, bx);
    Add axcx(ax, cx);
    //(ax + bx) + (ax + cx)
    Add ex(axbx, axcx);
    Graph g;
    //res = (a+b)*x + ax + cx
    Expression& res = f.Apply(ex);
    std::string s = g.Start(res);
    //res = (a+b)*x + (a+c)*x
    Expression& res2 = f.Apply(res);
    s = g.Start(res);
    //res = (a+b+a+c)*x
    Expression& res3 = f.Apply(res2);
    s = g.Start(res);

    MockVisitor v;
    EXPECT_CALL(v, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Add&>())).Times(3).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(a)))).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(b)))).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(c)))).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(x)))).Times(1).WillRepeatedly(Return(true));

    res3.Walk(v);
}

TEST(FactorTest, DeepEquality)
{
    FactoryFacade facade;
    FactorPattern f(facade);
    //create ax + bx
    Variable a;
    Variable b;
    Variable x;
    Variable y;
    Multiply ax(a, x);
    Multiply bx(b, x);
    Multiply ay(a, y);
    Multiply by(b, y);
    Add axbx(ax, bx);
    Add ayby(ay, by);
    //(ax + bx) + (ay + by)
    Add ex(axbx, ayby);
    //res = (a+b)*x + ay + by
    Expression& res = f.Apply(ex);
    //res = (a+b)*x + (a+b)*y
    Expression& res2 = f.Apply(res);
    //res = (a+b)*(x+y)
    Expression& res3 = f.Apply(res2);

    MockVisitor v;
    EXPECT_CALL(v, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Add&>())).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(a)))).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(b)))).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(x)))).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(Matcher<Variable&>(Ref(y)))).Times(1).WillRepeatedly(Return(true));

    res3.Walk(v);
}

TEST(FactorTest, FactorSum)
{
    FactoryFacade facade;
    FactorPattern f(facade);
    //create 2x + 3x + 4x
    Variable x;
    Scalar  two(2);
    Scalar  thre(3);
    Scalar  four(4);
    Multiply x2(two, x);
    Multiply x3(thre, x);
    Multiply x4(four, x);
    std::vector<Expression*> sumValues;
    sumValues.push_back(&x2);
    sumValues.push_back(&x3);
    sumValues.push_back(&x4);
    Sum sum(sumValues);
    //res = (2+3+4)*x
    Expression& res = f.Apply(sum);

    MockVisitor v;
    EXPECT_CALL(v, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Sum&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Scalar&>())).Times(3).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Variable&>())).Times(1).WillRepeatedly(Return(true));

    res.Walk(v);
}

TEST(FactorTest, PartialFactorSum)
{
    FactoryFacade facade;
    FactorPattern f(facade);
    //create 2x + 3x + 4
    Variable x;
    Scalar  two(2);
    Scalar  thre(3);
    Scalar  four(4);
    Multiply x2(two, x);
    Multiply x3(thre, x);
    std::vector<Expression*> sumValues;
    sumValues.push_back(&x2);
    sumValues.push_back(&x3);
    sumValues.push_back(&four);
    Sum sum(sumValues);
    //res = (2+3)*x + 4
    Expression& res = f.Apply(sum);

    MockVisitor v;
    EXPECT_CALL(v, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Sum&>())).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Scalar&>())).Times(3).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Variable&>())).Times(1).WillRepeatedly(Return(true));

    res.Walk(v);
}

TEST(FactorTest, SumNoFactor)
{
    FactoryFacade facade;
    FactorPattern f(facade);
    //create 2x + 3y + 4
    Variable x;
    Variable y;
    Scalar  two(2);
    Scalar  thre(3);
    Scalar  four(4);
    Multiply x2(two, x);
    Multiply y3(thre, y);
    std::vector<Expression*> sumValues;
    sumValues.push_back(&x2);
    sumValues.push_back(&y3);
    sumValues.push_back(&four);
    Sum sum(sumValues);
    //res = 2x + 3y + 4
    Expression& res = f.Apply(sum);

    MockVisitor v;
    EXPECT_CALL(v, Visit(An<Multiply&>())).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Sum&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Scalar&>())).Times(3).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Variable&>())).Times(2).WillRepeatedly(Return(true));

    res.Walk(v);
}

TEST(FactorTest, SumMultipleFactors)
{
    FactoryFacade facade;
    FactorPattern f(facade);
    //create ax + bx + ay + by
    Variable x;
    Variable y;
    Variable a;
    Variable b;
    Multiply ax(a, x);
    Multiply bx(b, x);
    Multiply ay(a, y);
    Multiply by(b, y);
    std::vector<Expression*> sumValues;
    sumValues.push_back(&ax);
    sumValues.push_back(&bx);
    sumValues.push_back(&ay);
    sumValues.push_back(&by);
    Sum sum(sumValues);
    //res = (x+y) * a + bx + by
    Expression& res = f.Apply(sum);
    //res = (x+y) * a + (x + y) * b
    Expression& res2 = f.Apply(res);
    //res = (x+y) * (a+b)
    Expression& res3 = f.Apply(res2);

    MockVisitor v;
    EXPECT_CALL(v, Visit(An<Multiply&>())).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Sum&>())).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(v, Visit(An<Variable&>())).Times(4).WillRepeatedly(Return(true));

    res3.Walk(v);
}