#include "Polynomial.h"

using namespace std;

Polynomial::Polynomial(string s, char var){
    terms = vector<Fraction>(1, Fraction(0));
    
    string coefStr = "", powerStr = "";
    int coef = 1, power = 0;
    
    //0 = getting coefficient 1 = getting power
    bool foundVar = 0;
    bool raisingPower = false;
    bool isNegative = false;

    for(int i=0; i<s.length(); i++){
        
        char c = s.at(i);
        
        cout<<"i: "<<i<<" States: "<<foundVar<<raisingPower<<isNegative<<"\n Strs: "<<coefStr<<" "<<powerStr<<endl;
        
        if(c == '-'){
            
            if(!powerStr.empty() || (foundVar && !raisingPower)){
                //Subtracting next term
                
                if(foundVar && !raisingPower) power = 1;
                else{
                    power = stoi(powerStr);
                    if(isNegative) power *= -1;
                    isNegative = false;
                }
                
                AddTerm(coef, power);
                
                //Reset
                isNegative = foundVar = raisingPower = 0;
                coefStr = ""; powerStr = "";
                coef = 1; power = 0;
            }
            
            isNegative = !isNegative;
        }
        else if(c == '+'){
            if(foundVar && !raisingPower) power = 1;
            else if(!powerStr.empty()){
                power = stoi(powerStr);
                if(isNegative) power *= -1;
            }
            
            AddTerm(coef, power);
            
            //Reset
            isNegative = foundVar = raisingPower = 0;
            coefStr = ""; powerStr = "";
            coef = 1; power = 0;
        }
        else if(isdigit(c)){
            if(foundVar) powerStr += c;
            else coefStr += c;
        }
        else if(c == var){
            foundVar = 1;
            
            if(!coefStr.empty()) coef = stoi(coefStr);
            
            if(isNegative) coef *= -1;
            isNegative = false;
        }
        else if(c == '^') raisingPower = 1;
    }
    //Catch last term
    if(foundVar){
        if (!raisingPower) power = 1;
        else if(!powerStr.empty()){
            power = stoi(powerStr);
            if(isNegative) power *= -1;
        }
    } else if(!coefStr.empty()){
        coef = stoi(coefStr);
        if(isNegative) coef *= -1;
    }
    
    AddTerm(coef, power);
    
    this->var = var;
}

Polynomial Polynomial::operator+(const Polynomial& other){
    int newHighest = max(highestPow, other.highestPow);
    
    Polynomial p = Polynomial(newHighest);
    
    for(int i=0; i<=newHighest; i++){
       p.AddTerm(GetTerm(i) + other.GetTerm(i), i);
    }
    
    return p;
}
Polynomial Polynomial::operator+=(const Polynomial& other){
    int newHighest = max(highestPow, other.highestPow);
    
    for(int i=0; i<=other.highestPow; i++){
        if(terms.at(i) == 0 && other.terms.at(i) != 0) numTerms++;
        terms.at(i) += other.terms.at(i);
        
    }
    
    return *this;
}

Polynomial Polynomial::operator*(const Polynomial& other){
    int newHighest = highestPow + other.highestPow;
    if(newHighest >= terms.size()) terms.resize(newHighest * 2, Fraction(0));
    
    Polynomial p = Polynomial(newHighest);
    
    for(int i=0; i<highestPow; i++){
        for(int j=0; j<other.highestPow; j++){
            p.AddTerm(GetTerm(i) * other.GetTerm(j), i+j);
        }
    }
    
    return p;
}
Polynomial Polynomial::operator*=(const Polynomial& other){
    numTerms = 0;

    //cout<<"d1 "<<highestPow<<" d2 "<<other.highestPow<<endl;
    vector<Fraction> newTerms(highestPow + other.highestPow + 1, Fraction(0));
    
    for(int i=0; i<=highestPow; i++){
        for(int j=0; j<=other.highestPow; j++){
            //cout<<i<<" "<<j<<endl;
            if(newTerms.at(i+j) == 0 && terms.at(i) != 0 && other.terms.at(j) != 0){
                numTerms++;
            }
            newTerms.at(i+j) += GetTerm(i) * other.GetTerm(j);
        }
    }
    
    highestPow += other.highestPow;
    terms = newTerms;
    
    return *this;
}

bool Polynomial::operator==(const Polynomial& other){
    if(numTerms != other.numTerms || highestPow != other.highestPow) return false;
    
    for(int i=0; i<=highestPow; i++){
        if(terms.at(i) != other.terms.at(i)) return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& obj) {
    if(obj.numTerms == 0){
        os<<"0";
        return os;
    }
    
    int termsLeft = obj.numTerms;
    
    if(obj.terms.at(obj.highestPow) != 0){
        if (obj.terms.at(obj.highestPow) != 1 || obj.highestPow == 0) os << obj.terms.at(obj.highestPow);
        if(obj.highestPow > 0) os <<obj.var;
        if (obj.highestPow > 1) os<<"^"<<obj.highestPow;
        
        if(obj.numTerms > 1) os << " + ";
        
        termsLeft--;
    }
    
    
    for(int i=obj.highestPow-1; i>=0; i--){
        if(termsLeft == 0) break;
        
        if(obj.terms.at(i) == 0) continue;
        
        if (obj.terms.at(i) != 1 || i == 0) os << obj.terms.at(i);
        if(i>0){
            os <<obj.var;
            if (i != 1) os<<"^"<<i;
        }
        
        //cout<<"print pow: "<<i<<" t left: "<<termsLeft<<endl;
        
        termsLeft--;
        if(termsLeft > 0) os<<" + ";
    }
    
    return os;
}


RepeatedPolynomial::RepeatedPolynomial(string s, char var){
   if(s.at(0) == '('){
       int closeParenthesis = s.find(')');
       polynomial = Polynomial(s.substr(1, closeParenthesis), var);
       
       //Power num should start 2 spots after ')'
       quantityPower = stoi(s.substr(closeParenthesis+2));
   }
   else{
       polynomial = Polynomial(s, var);
       quantityPower = 1;
       
       if(polynomial.numTerms == 1 && polynomial.highestPow > 1){
           //Convert to repeated poly
           quantityPower = polynomial.highestPow;
           
           polynomial.terms.at(1) = polynomial.terms.at(polynomial.highestPow);
           polynomial.terms.at(polynomial.highestPow) = 0;
           
           polynomial.highestPow = 1;
       }
   }
}
bool RepeatedPolynomial::operator==(const RepeatedPolynomial& other){
    if(quantityPower != other.quantityPower) return false;
    else return polynomial == other.polynomial;
}
std::ostream& operator<<(std::ostream& os, const RepeatedPolynomial& obj){
    if(obj.polynomial.numTerms == 0){
        os<<"0";
        return os;
    }
    
    if(obj.quantityPower > 1){
        if(obj.polynomial.numTerms == 1) os << obj.polynomial << "^"<<obj.quantityPower;
        else os << "(" << obj.polynomial << ")^"<<obj.quantityPower;
    }
    else os << obj.polynomial;
    
    return os;
}
