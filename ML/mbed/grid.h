#ifndef _GRID_H_
#define _GRID_H_

<<<<<<< HEAD
#include "MLX620_API.h"

//define constants
static const int scale = 5, inputRow=4, inputColumn=16;
static const double topCurve=.1, bottomCurve=-.1;
static bool polynomialApprox=false;

class GRID {
public:
    GRID(unsigned int r, unsigned int c);

    int getRow(){return row;};
    int getColumn(){return column;};

    double getValue(unsigned int r, unsigned int c);
    void setValue(unsigned int r, unsigned int c, double value);

    double getMax();
    unsigned int getMaxRowIndex();
    unsigned int getMaxColumnIndex();

    void calculateSumGrid(GRID *inputGrid, unsigned int size);

   //added by matt
    double GRID::avgSurrounding(int r, int c);
    void GRID::polyApprox(int r, int c);
    bool interpolateFrom(GRID *inputGrid, unsigned int scale);
    int animalLocStateless(unsigned int row, unsigned int column);

    void importGrid(double *inputArray);
    void exportGrid(double *outputArray);
    
=======
class GRID {
public:
    GRID(unsigned int r, unsigned int c);
    double getValue(unsigned int r, unsigned int c);
    void setValue(unsigned int r, unsigned int c, double value);
    double getMax();
    unsigned int getMaxRowIndex();
    unsigned int getMaxColumnIndex();
    void updateMaxInformation();
    void calculateSumGrid(GRID *inputGrid, unsigned int size);
    void cloneGrid(double *inputArray);

>>>>>>> origin/master
private:
    double *buffer;
    unsigned int length;
    bool maxSet;
    int row;
    int column;
    int maxRowIndex;
    int maxColumnIndex;
<<<<<<< HEAD
    void updateMaxInformation();
=======
>>>>>>> origin/master
};

GRID::GRID(unsigned int r, unsigned int c) {
    row = r;
    column = c;
    length = row * column;
    buffer = new double[length];
    maxSet = false;
}

double GRID::getValue(unsigned int r, unsigned int c) {
    return buffer[r * column + c];
}

void GRID::setValue(unsigned int r, unsigned int c, double value) {
    buffer[r * column + c] = value;
    maxSet = false;
}

void GRID::updateMaxInformation() {
    int i;
    int limit = row * column;
    int maxIndex = 0;
    double maxPotential = buffer[0];

    for (i = 1; i < limit; i++) {
        if (buffer[i] > maxPotential) {
            maxPotential = buffer[i];
            maxIndex = i;
        }
    }

    maxRowIndex = maxIndex / column;
    maxColumnIndex = maxIndex % column;
    maxSet = true;
}

double GRID::getMax() {
<<<<<<< HEAD
    if (maxSet == false) {
        updateMaxInformation();
    }

=======
>>>>>>> origin/master
    return getValue(maxRowIndex, maxColumnIndex);
}

unsigned int  GRID::getMaxRowIndex() {
<<<<<<< HEAD
    if (maxSet == false) {
        updateMaxInformation();
    }

=======
>>>>>>> origin/master
    return maxRowIndex;
}

unsigned int GRID::getMaxColumnIndex() {
<<<<<<< HEAD
    if (maxSet == false) {
        updateMaxInformation();
    }

=======
>>>>>>> origin/master
    return maxColumnIndex;
}

void GRID::calculateSumGrid(GRID *inputGrid, unsigned int size) {
    double temp;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            temp = 0;

            for (int k = 0; k < size; k++) {
                for (int l = 0; l < size; l++) {
                    temp += inputGrid->getValue(i + k, j + l);
                }
            }

            setValue(i, j, temp);
        }
    }
}

<<<<<<< HEAD
void GRID::importGrid(double *inputArray) {
=======
void GRID::cloneGrid(double *inputArray) {
>>>>>>> origin/master
    double temp;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {

<<<<<<< HEAD
            //setValue(i, j, inputArray[j * row + i]);
            setValue(i, j, inputArray[MLX620_IR_SENSOR_IDX(i, j)]);
=======
            setValue(i, j, inputArray[i * column + j]);
>>>>>>> origin/master
        }
    }
}

<<<<<<< HEAD
double GRID::avgSurrounding(int r, int c){
    unsigned int rowIndex, columnIndex;
    int i, j, count=0;
    double sum=0;
    
    //make sure index is inside 1 pixel rim of image
    if((r >= (row-1)) || (c >= (column-1))) return 0;
    if(r <=0 || c <= 0) return 0;
    
    //check surrounding pixels, and determine avg value (excluding 0)
    for(i=0; i<3; i++){
             for(j=0; j<3; j++){
                   rowIndex=r+j-1;
                   columnIndex=c+i-1;
                   
                   if(getValue(rowIndex, columnIndex) != 0){
                            count+=1;
                            sum+=getValue(rowIndex, columnIndex);
                   }
             }         
    }
    
    sum/=count;
    return sum;
}

void GRID::polyApprox(int r, int c){
     if(this->getValue(r,c) > (avg+threshold)) this->setValue(r,c,this->getValue(r,c)+topCurve*this->getValue(r,c));
     else if(this->getValue(r,c) < (avg-threshold)) this->setValue(r,c,this->getValue(r,c)+bottomCurve*this->getValue(r,c));
}

bool GRID::interpolateFrom(GRID *inputGrid, unsigned int scale)
{
    int i, j, k, l;
    double initPoint, finalPoint, increment, sum=0, maxTemp=-1, minTemp=100000;

    if ((((inputGrid->getColumn()-1)*scale + 1) != column) || 
        (((inputGrid->getRow()-1)*scale + 1) != row))  
    {
        //Not the right size
        return false;
    }
    else
    {
        //Init the current grid
        for (int i = 0; i < row; i++) {
             for (int j = 0; j < column; j++) {
                 setValue(i, j, 0);
             }
        }
        
        //Copy all known value
        for (int i = 0; i < inputGrid->getRow(); i++) {
             for (int j = 0; j < inputGrid->getColumn(); j++) {
                 setValue(i*scale, j*scale, inputGrid->getValue(i,j));
                 sum+=inputGrid->getValue(i,j);
                 if(inputGrid->getValue(i,j) > max) max=inputGrid->getValue(i,j);
                 if(inputGrid->getValue(i,j) < min) min=inputGrid->getValue(i,j);
             }
        }
        avg=sum/(inputGrid->getRow()*inputGrid->getColumn());
        threshold=.1*avg;

        
        //Interpolate all rows
         for (int i = 0; i < inputGrid->getRow(); i++) {
             for (int j = 0; j < inputGrid->getColumn()-1; j++) {
                 
                 initPoint  = inputGrid->getValue(i,j);
                 finalPoint = inputGrid->getValue(i,j+1);
                 increment  = (finalPoint - initPoint) / scale;

                 for (k=1; k < scale; k++) {
                     setValue(i*scale,j*scale+k,initPoint + increment*k);
                 }
             }
         }

         //Interpolate all columns
         for (int i = 0; i < inputGrid->getRow()- 1; i++) {
             for (int j = 0; j < inputGrid->getColumn(); j++) {

                 initPoint  = inputGrid->getValue(i,j);
                 finalPoint = inputGrid->getValue(i+1,j);
                 increment  = (finalPoint - initPoint) / scale;

                 for (k=1; k < scale; k++) {
                     setValue(i*scale+k, j*scale, initPoint + increment*k);
                 }
             }
         }
         
         //Interpolate the left Diagonal "\"
         for (int i = 0; i < inputGrid->getRow()- 1; i++) {
             for (int j = 0; j < inputGrid->getColumn()-1; j++) {

                 initPoint  = inputGrid->getValue(i,j);
                 finalPoint = inputGrid->getValue(i+1,j+1);
                 increment  = (finalPoint - initPoint) / scale;

                 for (k=1; k < scale; k++) {
                     setValue(i*scale+k,j*scale+k,initPoint + increment*k);
                 }
             }
         }
         
         //Interpolate the right Diagonal "/"
         for (int i = 0; i < inputGrid->getRow()- 1; i++) {
             for (int j = 0; j < inputGrid->getColumn()-1; j++) {

                 initPoint  = inputGrid->getValue(i+1,j);
                 finalPoint = inputGrid->getValue(i,j+1);
                 increment  = (finalPoint - initPoint) / scale;

                 for (k=1; k < scale; k++) {
                     setValue(i*scale-k,(j+1)*scale-k,initPoint + increment*k);
                 }
             }
         }
         
         //For any point that is not filled in yet, determine approx. average
         for (int i = 0; i < row; i++) {
             for (int j = 0; j < column; j++) {
                 if(getValue(i,j) == 0) setValue(i,j,avgSurrounding(i,j));
             }
         }
/*
         //Interpolate the row again
         for (int i = 1; i < getRow() - 1; i++) {
             for (int j = 0; j < getColumn()-1; j+=scale) {

                 double initPoint  = getValue(i,j);
                 double finalPoint = getValue(i,j+scale);
                 double increment  = (finalPoint - initPoint) / (scale+1);

                 for (k=1; k < scale;k++) {
                     setValue(i,j+k,initPoint + increment*k);
                 }
             }
         }

         //Interpolate the column again
         for (int i = 1; i < getRow() - 1; i+=scale) {
             for (int j = 0; j < getColumn()-1; j++) {

                 double initPoint  = getValue(i,j);
                 double finalPoint = getValue(i+scale,j);
                 double increment  = (finalPoint - initPoint) / (scale+1);

                 for (k=1; k < scale;k++) {
                     double temp = ((initPoint + increment*k) + getValue(i,j+k)) / 2; 
                     setValue(i,j+k,temp);
                 }
             }
         }
*/
        //Done 
        return true;
    }
}

int GRID::animalLocStateless(unsigned int row, unsigned int column){
    row = getMaxRowIndex();
    column = getMaxColumnIndex();    
}

=======
>>>>>>> origin/master

#endif
