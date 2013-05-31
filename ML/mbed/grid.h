#ifndef _GRID_H_
#define _GRID_H_

<<<<<<< HEAD
#include "MLX620_API.h"

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
    bool interpolateFrom(GRID *inputGrid, unsigned int scale);

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
void GRID::exportGrid(double *outputArray) {
    double temp;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {

            outputArray[i * column + j] = getValue(i, j);
        }
    }
}

bool GRID::interpolateFrom(GRID *inputGrid, unsigned int scale)
{
    int i, j, k,l;

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
             }
        }

         //Interpolate the row
         for (int i = 0; i < inputGrid->getRow(); i++) {
             for (int j = 0; j < inputGrid->getColumn()-1; j++) {

                 double initPoint  = inputGrid->getValue(i,j);
                 double finalPoint = inputGrid->getValue(i,j+1);
                 double increment  = (finalPoint - initPoint) / scale;

                 for (k=1; k < scale;k++) {
                     setValue(i*scale,j*scale+k,initPoint + increment*k);
                 }
             }
         }

         //Interpolate the column
         for (int i = 0; i < inputGrid->getRow()- 1; i++) {
             for (int j = 0; j < inputGrid->getColumn(); j++) {

                 double initPoint  = inputGrid->getValue(i,j);
                 double finalPoint = inputGrid->getValue(i+1,j);
                 double increment  = (finalPoint - initPoint) / scale;

                 for (k=1; k < scale;k++) {
                     setValue(i*scale+k,j*scale,initPoint + increment*k);
                 }
             }
         }

         //Interpolate the row again
         for (int i = 1; i < getRow() - 1; i++) {
             for (int j = 0; j < getColumn()-1; j+=scale) {

                 double initPoint  = getValue(i,j);
                 double finalPoint = getValue(i,j+scale);
                 double increment  = (finalPoint - initPoint) / scale;

                 for (k=1; k < scale;k++) {
                     setValue(i,j+k,initPoint + increment*k);
                 }
             }
         }

         //Interpolate the column again
         for (int i = 0; i < getRow() - 1; i+=scale) {
             for (int j = 1; j < getColumn()-1; j++) {

                 double initPoint  = getValue(i,j);
                 double finalPoint = getValue(i+scale,j);
                 double increment  = (finalPoint - initPoint) / scale;

                 for (k=1; k < scale;k++) {
                     double temp = ((initPoint + increment*k) + getValue(i+k,j)) / 2.0; 
                     setValue(i+k,j,temp);
                     //setValue(i,j,(initPoint + increment*k));
                 }
             }
         }

        //Done 
        return true;
    }

}

=======
>>>>>>> origin/master

#endif
