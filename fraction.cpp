#include "fraction.h"

void Fraction::simplify(){
    if(numerator == 0){
        //0 / anything == 0
        if(!denominator == 0) denominator = 1;
    }
    else{
        int gcf = std::gcd(numerator, denominator);
        
        numerator /= gcf;
        denominator /= gcf;
        
        //Keep denominator positive
        if(denominator < 0){
            numerator *= -1;
            denominator *= -1;
        }
    }
}

//Support fraction addition, subtraction, *, /
Fraction Fraction::operator+(const Fraction& other){
    if(other.denominator == denominator) return Fraction(numerator + other.numerator, denominator);
    else{
        numerator = numerator * other.denominator + other.numerator * denominator;
        denominator =  denominator * other.denominator;
    }
    
    //Simplify :)
    simplify();
    
    return *this;
}
Fraction Fraction::operator-(const Fraction& other){
    if(other.denominator == denominator) return Fraction(numerator + other.numerator, denominator);
    else{
        numerator = numerator * other.denominator - other.numerator * denominator;
        denominator =  denominator * other.denominator;
        
        
    }
    
    //Simplify :)
    simplify();
    
    return *this;
}
Fraction Fraction::operator*(const Fraction& other){ 
    Fraction f = Fraction(numerator * other.numerator, denominator * other.denominator);;
    f.simplify();
    
    return f; 
}
Fraction Fraction::operator/(const Fraction& other){
    Fraction f =  Fraction(numerator * other.denominator, denominator * other.numerator);
    f.simplify();
    
    return f; 
}

Fraction& Fraction::operator+=(const Fraction& other){
    if(other.denominator == denominator){
        numerator += other.numerator;
    }
    else{
        numerator = numerator * other.denominator + other.numerator * denominator;
        denominator =  denominator * other.denominator;
    }
    
    simplify();
    
    return *this;
}
Fraction& Fraction::operator*=(const Fraction& other){
    numerator *= other.numerator;
    denominator *= other.denominator;
    
    simplify();
    
    return *this;
}

ostream& operator<<(ostream& os, const Fraction& obj){
    obj.print();
    return os;
}
