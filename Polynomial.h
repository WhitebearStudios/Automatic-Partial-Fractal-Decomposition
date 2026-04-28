#include <vector>
#include <iostream>
#include "fraction.h"

using namespace std;

#ifndef POLY_H
#define POLY_H

struct Polynomial{
    //element 0 is 0th power of var, 1 is 1st, etc
    vector<Fraction> terms;
    
    int highestPow = 0;
    
    char var = 'x';
    
    int numTerms = 0;
    
    Polynomial(){
        highestPow = 0;
        terms = vector<Fraction>(1, Fraction(0));
    }
    Polynomial(int highestPow){
        this->highestPow = highestPow;
        numTerms = 0;
        
        terms = vector<Fraction>(highestPow+1, Fraction(0));
    }
    Polynomial(string s, char var);
    
    Fraction GetTerm(int power) const{
        if(power > highestPow) return 0;
        
        return terms.at(power);
    }
    
    void AddTerm(Fraction coef, int power){
        if(power > highestPow){
            terms.resize(power * 2, Fraction(0));
            highestPow = power;
        }
        
        cout<<"Added term! Power "<<power<<" coef: "<<coef<<endl;
        
        if(terms.at(power) == 0 && coef != 0) numTerms++;
        
        terms.at(power) += coef;
    }
    
    
    
    
    Polynomial operator+(const Polynomial& other);
    Polynomial operator+=(const Polynomial& other);
    
    Polynomial operator*(const Polynomial& other);
    Polynomial operator*=(const Polynomial& other);
    
    bool operator==(const Polynomial& other);
    
    friend ostream& operator<<(ostream& os, const Polynomial& obj);
    
    
};

struct RepeatedPolynomial{
   int quantityPower; 

   Polynomial polynomial;
   
   RepeatedPolynomial() {
       polynomial = Polynomial();
       quantityPower = -1;
   }
   RepeatedPolynomial(Polynomial p, int power){
       polynomial = p;
       quantityPower = power;
   }
   RepeatedPolynomial(string s, char var);
   
   Polynomial GetExpanded(){
       Polynomial p = polynomial;
       for(int i=0; i<quantityPower-1; i++){
           p *= polynomial;
       }
       
       return p;
   }
   
   
   bool operator==(const RepeatedPolynomial& other);
   
   friend ostream& operator<<(ostream& os, const RepeatedPolynomial& obj);
};

#endif
