#ifndef _GRID_H_
#define _GRID_H_

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
    
    double getAvg();
    

    void calculateSumGrid(GRID *inputGrid, unsigned int size);
    bool interpolateFrom(GRID *inputGrid, unsigned int scale);
    
    bool gridAddition(GRID * inputGrid);
    void gridScalarAddition(double scalar);
    void gridScalarMultiplication(double scalar);

    bool importGridArray(GRID ** inputArrayList, int number);
    void importGrid(double *inputArray);
    void exportGrid(double *outputArray);
    
private:
    double *buffer;
    unsigned int length;
    bool maxSet;
    bool avgSet;
    
    double gridAvg;    
    int row;
    int column;
    int maxRowIndex;
    int maxColumnIndex;
    void updateAvgInformation();
    void updateMaxInformation();
};


#endif

