#include "matrix.h"

#ifndef MATRIX_SOLVER_H
#define MATRIX_SOLVER_H

class MatrixSolver{
    public:
        //Precondition: leading entry of each row must be in same or left col of LE of row below
        static void echelonForm(RowEquMatrix& matr, bool showSteps = true, bool scaleRows = true){
            //echelon form w/o scaling: don't do anything to 1st row
            if(!scaleRows) matr.reducedRows[0] = true;
            
            
            
            //Keep rows close to echelon form to prevent bugs
            matr.sort();
            if(showSteps) {cout << "\nSorted:"<<endl;
            matr.print();}
            
            int c = 0;
            for(int r=0; r<matr.getNumRows(); r++){
                if(showSteps) cout << "At row: "<<r <<"\n\n";
                
                //cout << "Get entry at "<<r<<","<<c<<endl;
                Fraction thisEntry = matr.getEntry(r,c);
                
                //Inrease c to get non-zero entry
                while(thisEntry == 0 && c < matr.getNumCols()){
                    c++;
                    if(c < matr.getNumCols()) thisEntry = matr.getEntry(r,c);
                    else break;
                }
                if(c == matr.getNumCols()){
                    cout << "All further rows are 0s"<<endl;
                    break;
                }
                if(showSteps) cout << "At col: "<<c<<endl;

                
                //scale leadng entry so 1
                if(scaleRows){
                    matr.scaleRow(thisEntry.reciprocal(), r);
                    
                    if(showSteps) {cout << "Scale row " << r << " to 1:" << endl;
                    matr.print();}
                }
                
                int r2 = matr.getNumRows() - 1;
                
                Fraction entryToCancelOthers = matr.getEntry(r,c);
                
                //Turn entries below LE to 0s
                while(r2 > r){
                    thisEntry = matr.getEntry(r2,c);
                    if(showSteps){cout << "Multiply row " << (r+1) << " by ";
                    (-thisEntry / entryToCancelOthers).print();
                    cout << " and add to row " << (r2+1) << " to make col " << (c+1) << " a 0:" << endl;}
                    
                    matr.multAndAdd(r, -thisEntry / entryToCancelOthers, r2);
                    if(showSteps) matr.print();
                    
                    //Sort to make sure funny cancellation doesn't break anything
                    matr.sort();
                    if(showSteps) {cout << "\nSorted:"<<endl;
                    matr.print();}
                    
                    r2--;
                }
            }
        }
        
        //Precondition: matrix is in echelon form and all leading entries = 1
        static void reducedEchelonForm(RowEquMatrix& matr, bool showSteps = true){
            for(int r=matr.getNumRows()-1; r>=0; r--){
                //Find leading entry
                int c=0;
                Fraction thisEntry = Fraction(0,1);
                while(thisEntry == 0){
                    c++;
                    if(c == matr.getNumCols()) break;
                    
                    thisEntry = matr.getEntry(r,c);
                    /*cout << "At " << r << "," << c << ":";
                    thisEntry.print();
                    cout << endl;*/
                }
                
                if(thisEntry==0) continue; //This row is all 0s
                
                //Reduce above rows
                for(int i=r-1; i>=0; i--){
                    if(showSteps) {cout << "Multiply row " << (r+1) << " by ";
                    (-matr.getEntry(i, c)).print();
                    cout << " and add to row " << (i+1) << " to make col " << (c+1) << " a 0:" << endl;}
                    
                    matr.multAndAdd(r, -matr.getEntry(i, c), i);
                    if(showSteps) matr.print();
                }
            }
        }
};

#endif
