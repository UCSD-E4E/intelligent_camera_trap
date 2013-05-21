#ifndef _GRID_H_
#define _GRID_H_

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

private:
    double *buffer;
    unsigned int length;
    bool maxSet;
    int row;
    int column;
    int maxRowIndex;
    int maxColumnIndex;
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
    return getValue(maxRowIndex, maxColumnIndex);
}

unsigned int  GRID::getMaxRowIndex() {
    return maxRowIndex;
}

unsigned int GRID::getMaxColumnIndex() {
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

void GRID::cloneGrid(double *inputArray) {
    double temp;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {

            setValue(i, j, inputArray[i * column + j]);
        }
    }
}


#endif
