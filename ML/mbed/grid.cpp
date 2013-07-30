#include "grid.h"
#include "MLX620_API.h"

GRID::GRID(unsigned int r, unsigned int c) {
    row = r;
    column = c;
    length = row * column;
    buffer = new double[length];
    maxSet = false;
    avgSet = false;
}

double GRID::getValue(unsigned int r, unsigned int c) {
    return buffer[r * column + c];
}

void GRID::setValue(unsigned int r, unsigned int c, double value) {
    buffer[r * column + c] = value;
    maxSet = false;
    avgSet = false;
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

void GRID::updateAvgInformation() {
    
    double sum = 0 ;
    
    for (int i = 0; i < row * column ; i++) {
        
        sum += buffer[i];    
        
    }

    gridAvg = sum / (row * column) ;    
    avgSet = true;
}

double GRID::getMax() {
    if (maxSet == false) {
        updateMaxInformation();
    }

    return getValue(maxRowIndex, maxColumnIndex);
}

double GRID::getAvg() {
    if (avgSet == false) {
        updateAvgInformation();
    }

    return gridAvg;
}

unsigned int  GRID::getMaxRowIndex() {
    if (maxSet == false) {
        updateMaxInformation();
    }

    return maxRowIndex;
}

unsigned int GRID::getMaxColumnIndex() {
    if (maxSet == false) {
        updateMaxInformation();
    }

    return maxColumnIndex;
}

bool GRID::gridAddition(GRID * inputGrid)
{
    if (row                    !=  inputGrid->getRow() ||
        column                 !=  inputGrid->getColumn() )
    {
        return false;
    }
    else
    {
      for (int i = 0; i < row ; i++) {
        for (int j = 0; j < column ; j++) {
        
            setValue(i,j, inputGrid->getValue(i,j) + getValue(i,j)) ;
        
        }
      }
    }
    
    return true;
}

bool GRID::importGridArray(GRID ** gridArray, int number)
{
        int segmentRowNumber = gridArray[0]->getRow();
        
        if (row                    !=  segmentRowNumber * number ||
            column                 !=  gridArray[0]->getColumn() )
        {
            return false;
        }
        else
        {
            for (int i = 0; i < number; i++) {
              for (int j = 0; j < row / number; j++) {
                for (int k = 0; k < column; k++) {

                 setValue(j + (i*segmentRowNumber), k, gridArray[i]->getValue(j,k) );
                 }
               }
            }        
         }
    
    return true;

}

void GRID::gridScalarAddition(double scalar)
{
      for (int i = 0; i < row ; i++) {
        for (int j = 0; j < column ; j++) {
        
            setValue(i,j, getValue(i,j) + scalar) ;
        
        }
      }
}

void GRID::gridScalarMultiplication(double scalar)
{
      for (int i = 0; i < row ; i++) {
        for (int j = 0; j < column ; j++) {
        
            setValue(i,j, getValue(i,j) * scalar) ;
        
        }
      }
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

void GRID::importGrid(double *inputArray) {

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {

            setValue(i, j, inputArray[MLX620_IR_SENSOR_IDX(i, j)]);
        }
    }
}

void GRID::exportGrid(double *outputArray) {
    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {

            outputArray[i * column + j] = getValue(i, j);
        }
    }
}

bool GRID::interpolateFrom(GRID *inputGrid, unsigned int scale)
{
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

                 for (int k=1; k < scale;k++) {
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

                 for (int k=1; k < scale;k++) {
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

                 for (int k=1; k < scale;k++) {
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

                 for (int k=1; k < scale;k++) {
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
