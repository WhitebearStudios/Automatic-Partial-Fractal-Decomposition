#include "matrixSolver.h"
#include "Polynomial.h"



int main() {
    //INPUT:
    //The variable in question
    //Denominators of separated fractions. The algorithm will calculate the numerators.
    //The numerator of the original fraction which the algorithm will decompose

    
    char var;
    cout << "Enter the variable in question: ";
    cin >> var;
    
    int numFracs, maxSolutionDegree = 0;
    cout << "\n-Don't repeat factors like s^2 -> s and s^2; the program will do it.\n\nHow many fractions are you decomposing into?"<<endl;
    cin >> numFracs;
    
    vector<RepeatedPolynomial> denoms(numFracs);
    Polynomial numer;
    
    cout<<"Enter the denominators of the decomposed fractions.\n-First will be the left-most.\n-For exponents, type x^2, 2x^3, etc.\n-Type 3x for 3 times x. No *\n-Only simple repeated polynomials are valid like (x+1)^3."<<endl;
    cin.ignore();
    string s;
    for(int i=0; i<numFracs; i++){
        
        cout<<"Denom "<<(i+1)<<": ";
        getline(cin, s);
        
        denoms.at(i) = RepeatedPolynomial(s, var);
        //Add factors of repeated denoms
        if(denoms.at(i).quantityPower > 1){
            for(int j=denoms.at(i).quantityPower-1; j>0; j--){
                denoms.push_back(RepeatedPolynomial(denoms.at(i).polynomial, j));
            }
        }
        
        maxSolutionDegree += denoms.at(i).quantityPower * denoms.at(i).polynomial.highestPow;
    }

    cout<<"Finally, enter the numerator of the original fraction."<<endl;
    
    getline(cin, s);
    numer = Polynomial(s, var);
    
    cout<<"\n-------\n Your data:\nVar: "<<var<<"\nDenominators: "<<endl;
    for(auto p : denoms){
        cout<<p<<", ";
    }
    cout<<"\nNumerator: "<<numer<<"\n---------------\n"<<endl;
    
    //Get common denominators
    vector<Polynomial> numerators, commonizedNumerators;
    int numeratorGroupIndex = 0;
    //The denoms from [0-numFracs) are the factors of the common denom so traverse those first
    for(auto p : denoms){
        cout<<"---P: "<<p<<endl;
        
        //Set up the numerator: single factor is A, repeated is Bs+C, etc
        int numeratorGroupSize = p.polynomial.highestPow;
        for(int i=numeratorGroupSize-1; i>=0; i--){
            cout<<"ni: "<<i<<endl;
            Polynomial numerator = Polynomial(i);
            numerator.AddTerm(1, i);
            
            
            
            numerators.push_back(numerator);
            commonizedNumerators.push_back(numerator);
            
            cout<<"New numerator: "<<numerators.at(numerators.size()-1)<<endl;
        }
        
        for(int f2=0;f2<numFracs; f2++){
            RepeatedPolynomial p2 = denoms.at(f2);
            if(p == p2){
                cout<<"Skip "<<p2<<endl;
                continue;
            }
            else cout<<"* with "<<p2<<endl;
            
            //If this denom p2 is a factor of p we don't need 
            //to multiply by it to get a common denom
            //if(p.quantityPower > p2.quantityPower && p.polynomial == p2.polynomial) continue;
            
            //Multiply the numerator group
            for(int i=numeratorGroupIndex; i<numeratorGroupIndex+numeratorGroupSize; i++){
                
                //This should only be true when p is a factor of p2
                if(p.polynomial == p2.polynomial){
                    //p is a factor of p2 so only multiply enough to get to p2
                    p2.quantityPower -= p.quantityPower;
                }
                cout<<"i: "<<i<<endl;
                cout<<commonizedNumerators.at(i)<<" *= "<<p2.GetExpanded()<<endl;
                commonizedNumerators.at(i) *= p2.GetExpanded();
            }
        }
        
        cout<<"Processed Numerators: "<<endl;
        for(int i=numeratorGroupIndex; i<numeratorGroupIndex+numeratorGroupSize; i++){
            cout<<commonizedNumerators.at(i)<<endl;
        }
        cout<<"\n\n"<<endl;
        
        numeratorGroupIndex += numeratorGroupSize;
    }
    
    //Calculate how the variables ABC influence each degree of x and fill in matrix
    RowEquMatrix matrix = RowEquMatrix(maxSolutionDegree, commonizedNumerators.size() + 1);
    
    for(int i=0; i<maxSolutionDegree; i++){
        for(int j=0; j<commonizedNumerators.size(); j++){
            matrix.setEntry(i, j, commonizedNumerators.at(j).GetTerm(i));
        }
        
        //Add desired coefficient of numerator
        matrix.setEntry(i, commonizedNumerators.size(), numer.GetTerm(i));
    }
    
    cout<<"Solution matrix: "<<endl;
    matrix.print();
    
    //Solve!!
    MatrixSolver::echelonForm(matrix, false);
    MatrixSolver::reducedEchelonForm(matrix, false);
    
    cout<<"Solved matrix: "<<endl;
    matrix.print();
    
    //Plug back into numerators which only have 1 term
    for(int i=0; i<numerators.size(); i++){
        numerators.at(i).terms.at(numerators.at(i).highestPow) = matrix.getEntry(i, numerators.size());
        
        //cout<<"New numer: "<<numerators.at(i)<<endl;
    }
    
    //Print answer
    cout<<"\n\n----Final answer:"<<endl;
    numeratorGroupIndex = 0;
    for(auto p : denoms){
        int numeratorGroupSize = p.polynomial.highestPow;
        Polynomial finalNumerator = Polynomial(numeratorGroupSize);
        for(int j=numeratorGroupSize-1; j>=0; j--){
            finalNumerator += numerators.at(numeratorGroupIndex);
            
            numeratorGroupIndex++;
        }
        
        cout << finalNumerator << "\n-------------  +  \n"<<p<<"\n"<<endl;
    }
    
    return 0;
}
