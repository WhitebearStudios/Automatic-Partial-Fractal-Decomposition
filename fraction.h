#include <numeric>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

#ifndef FRAC_H
#define FRAC_H

class Fraction{
    private:
        int numerator, denominator;
        
    public:
        int getNumerator() const{
            return numerator;
        }
        int getDenominator() const{
            return denominator;
        }
        
        double decimal() const{
            return (double)numerator / denominator;
        }
        
        Fraction reciprocal() const{
            return Fraction(denominator, numerator);
        }
        void simplify();
        
        //Support fraction addition, subtraction, *, /
        Fraction operator+(const Fraction& other);
        Fraction operator-(const Fraction& other);
        Fraction operator*(const Fraction& other);
        Fraction operator/(const Fraction& other);
        
        Fraction& operator+=(const Fraction& other);
        Fraction& operator*=(const Fraction& other);
        Fraction operator-(){
            return Fraction(-numerator, denominator);
        }
        
        bool operator==(const Fraction& other) const{
            return numerator == other.numerator && denominator == other.denominator;
        }
        bool operator==(const int other) const{
            Fraction cpy = Fraction(numerator, denominator);
            cpy.simplify();
            return cpy.numerator == other && cpy.denominator == 1;
        }
        bool operator==(const double other) const{
            return other == decimal();
        }
        bool operator==(const float other) const{
            return other == decimal();
        }
        bool operator!=(const Fraction& other) const{
            return !(*this == other);
        }
        bool operator!=(const int other) const{
            return !(*this == other);
        }
        bool operator!=(const double other) const{
            return other != decimal();
        }
        bool operator!=(const float other) const{
            return other != decimal();
        }
        
        /*operator int() const {
            return static_cast<int>(decimal());
        }*/
        
        
        void print() const{
            if(denominator == 1) cout << numerator;
            else cout << numerator << "/" << denominator;
        }
        
        
        Fraction(int numerator, int denominator){
            this->numerator = numerator;
            this->denominator = denominator;
        }
        Fraction(int val){
            numerator = val;
            denominator = 1;
        }
        
        friend ostream& operator<<(ostream& os, const Fraction& obj);
};

#endif
