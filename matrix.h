#include "fraction.h"

#ifndef MATRIX_H
#define MATRIX_H

class Matrix{
    protected:
        int numRows, numCols;
        
        vector<vector<Fraction>> matrix;
        
        //Assign a non-empty matrix
        void assignArr(vector<vector<Fraction>> matrix){
            this->matrix = matrix;
            
            
            numRows = matrix.size();
            numCols = matrix.at(0).size();
        }
        void addRow(vector<Fraction> row){
            matrix.push_back(row);
        }
        
    public:
        vector<vector<Fraction>> getArr(){ return matrix; }
    
        int getNumRows(){
            return numRows;
        }
        int getNumCols(){
            return numCols;
        }
        
        Fraction getEntry(int r, int c){
            //cout << "Fetch "<<r<<","<<c<<" size "<<matrix.size()<<","<<matrix.at(0).size()<<endl;
            return matrix.at(r).at(c);
        }
        void setEntry(int r, int c, Fraction entry){
            matrix.at(r).at(c) = entry;
        }
        
        
        void print(){
            cout << "[ ";
            
            for(int r = 0; r < numRows; r++){
                for(int c = 0; c < numCols; c++){
                    getEntry(r, c).print();
                    cout << ", ";
                }
                //Newline if not last row
                if(r < numRows - 1) cout << "\n  ";
            }
            
            cout << " ]" << endl;
        }
        
        
        Matrix(int numRows, int numCols){
            this->numRows = numRows;
            this->numCols = numCols;
            
            //Fill matrix with zeros so undefined
            //0/0 will be undefined
            for(int r = 0; r < numRows; r++){
                vector<Fraction> tmp;
                for(int c = 0; c < numCols; c++){
                    tmp.push_back(Fraction(0,0));
                }
                matrix.push_back(tmp);
            }
        }
        Matrix(Matrix& m){
            assignArr(m.getArr());
        }
        Matrix(){}; //Default constructor
};

class RowEquMatrix : public Matrix{
    public:
        int numSwaps = 0;
        //Keep track of which rows have been reduced
        bool reducedRows[];
    
        RowEquMatrix(int numRows, int numCols) : Matrix(numRows, numCols){}
        /*RowEquMatrix(string s){
            //Split along space

            stringstream ss(s);
            string token;
            int intsFound = 0;
            int n;
            bool tokenHasInt;
            while (getline(ss, token, ' ')) {
                tokenHasInt = true;
                
                cout<<token<<endl;
                
                try{
                    n = stoi(token);
                }
                catch(const std::invalid_argument& e){tokenHasInt = false;}
                
                if(tokenHasInt && to_string(tokenHasInt).size() == token.size()){
                    if(intsFound == 0) numRows = n;
                    else if (intsFound == 1){
                        numCols = n;
                        
                        //Fill vectors
                        //Fill matrix with zeros so undefined
                        //0/0 will be undefined
                        for(int r = 0; r < numRows; r++){
                            vector<Fraction> tmp;
                            for(int c = 0; c < numCols; c++){
                                tmp.push_back(Fraction(0, 0));
                            }
                            matrix.push_back(tmp);
                        }
                    }
                    else{
                        int ePos = intsFound - 2;
                        cout<<"Index: "<<ePos<<": "<<n<<endl;
                        setEntry(ePos / numCols, ePos % numCols, Fraction(n));
                    }
                    intsFound++;
                }
                
            }
            
            
        }
        */
        void multAndAdd(int fromRow, double mult, int toRow){multAndAdd(fromRow, Fraction(mult), toRow);}
        void multAndAdd(int fromRow, Fraction mult, int toRow){
            //cout << "* row " << fromRow << " by " << mult.decimal() << " and add that to row " << toRow << endl;
            for(int i=0; i<numCols; i++){
                matrix.at(toRow).at(i) += matrix.at(fromRow).at(i) * mult;
            }
            
            reducedRows[toRow] = true;
        }
        void scaleRow(double scale, int row) {scaleRow(Fraction(scale), row);}
        void scaleRow(Fraction scale, int row){
            for(int i=0; i<numCols; i++){
                matrix.at(row).at(i) *= scale;
            }
            
            reducedRows[row] = true;
        }
        
        //Sorts rows to get closer to echelon form
        void sort(){
            //# of zeros before leading entry in each row
            int priority[numRows];
            
            for(int r=0; r<numRows; r++){
                int c=0;
                while(getEntry(r,c) == 0){
                    c++;
                    if(c == numCols) break;
                }
                
                priority[r] = c;
                //cout << "Row " << r << ": " << c << endl;
            }
            
            //Insertion sort
            Matrix copy = Matrix(*this);
            matrix.clear();
            
            //Add a row to empty matrix
            addRow(copy.getArr().at(0));
            vector<int> priorities = {priority[0]};
            
            for(int r=1; r<copy.getNumRows(); r++){
                //Figure out where to insert
                int i=0;
                while(i<priorities.size()){
                    if(priorities.at(i) > priority[r]) break;
                    i++;
                }
                //Insert
                matrix.insert(matrix.begin()+i, copy.getArr().at(r));
                priorities.insert(priorities.begin()+i, priority[r]);
                
                swap(i, r);
            }
            
            //cout <<"Sorted: "<<endl;
            //print();
            //cout<<endl;
        }
    protected:
        //Helper for matrix solver
        void swap(int a, int b){
            bool tmp = reducedRows[a];
            reducedRows[a] = reducedRows[b];
            reducedRows[b] = tmp;
            
            numSwaps++;
        }
};

#endif
