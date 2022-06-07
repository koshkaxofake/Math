#pragma once

#include <vector>

class Rational
{
public:
    Rational() :numerator(0), denominator(1) { simplify(); }
    Rational(int value) :numerator(value), denominator(1) { simplify(); }
    Rational(long value) :numerator(value), denominator(1) { simplify(); }
    Rational(int value, int value2) :numerator(value), denominator(value2) { simplify(); }
    Rational(long long value, long long value2) :numerator(value), denominator(value2) { simplify(); }
    long double getValue() const
    {
        return numerator / denominator;
    }

    long long GCD(long long A, long long B) const
    {
        if (A == 0)
            return B;
        if (B == 0)
            return A;
        long long R = A % B;
        return GCD(B, R);
    }

    void simplify()
    {
        if (numerator == denominator)
        {
            numerator = 1;
            denominator = 1;
            return;
        }
        //fix signs(so top numerator is the signed one)
        if (denominator < 0)
        {
            numerator *= -1;
            denominator *= -1;
        }
        long long divisor = GCD(numerator, denominator);
        numerator /= divisor;
        denominator /= divisor;
    }

    bool operator==(const Rational& rhs) const
    {
        Rational temp = rhs;
        return temp.numerator == numerator && temp.denominator == denominator;
    }
    Rational& operator*=(long long rhs)
    {
        numerator *= rhs;
        simplify();
        return *this;
    }
    Rational& operator*(long long rhs) const
    {
        Rational temp = *this;
        return temp *= rhs;
    }
    Rational& operator/=(long long rhs)
    {
        denominator *= rhs;
        simplify();
        return *this;
    }
    Rational& operator/(long long rhs) const
    {
        Rational temp = *this;
        return temp /= rhs;
    }
    Rational& operator/=(const Rational& rhs)
    {
        numerator *= rhs.denominator;
        denominator *= rhs.numerator;
        simplify();
        return *this;
    }
    Rational& operator/(const Rational& rhs) const
    {
        Rational temp = *this;
        return temp /= rhs;
    }
    Rational& operator*=(const Rational& rhs)
    {
        numerator *= rhs.numerator;
        denominator *= rhs.denominator;
        simplify();
        return *this;
    }
    Rational& operator*(const Rational& rhs) const
    {
        Rational temp = *this;
        return temp *= rhs;
    }
    Rational& operator+=(const Rational &rhs)
    {
        Rational temp = rhs;
        long long oldDenom = temp.denominator;
        temp.numerator *= denominator;
        temp.denominator *= denominator;
        numerator *= oldDenom;
        denominator *= oldDenom;
        numerator += temp.numerator;
        simplify();
        return *this;
    }
    Rational& operator+(const Rational& rhs) const
    {
        Rational temp = *this;
        return temp += rhs;
    }
    Rational& operator-=(const Rational& rhs)
    {
        Rational temp = rhs*-1;
        *this += temp;
        return *this;
    }
    Rational& operator-(const Rational& rhs) const
    {
        Rational temp = *this;
        return temp -= rhs;
    }
    bool operator<(const Rational &rhs) const
    {
      //todo this can be inaccurate at large values, add some extra checks for when at large values
      return numerator * rhs.denominator < rhs.numerator* denominator;
    }
    bool operator>(const Rational& rhs) const
    {
      return rhs < *this;
    }

private:
    long long numerator;
    long long denominator;
};