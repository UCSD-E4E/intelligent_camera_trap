#include "interpolation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

static double input[inputPixels], output[outputPixels];

//function declaration
void interpolation(double[], double[]);
void printInput();
void printOutput();

int main(){
     int i, j, k;
    
     for(i=0; i<inputPixels; i++) input[i]=90;
     
     input[0]=99;
     
     interpolation(input, output);
     printInput();
     printOutput();
     system("pause");
     return 0;     
}

void interpolation(double input[], double output[]){
     int i, j, k, temp;
     double difference, slope, increment;
     
     //initialize output array with 0 values
     for(i=0; i < outputPixels; i++){
           output[i] = 0;
     }
     
     if(polinomialApprox){
          //polinomial approximation used
          printf("polinomial approx. not impletented yet...");
     }
     else{
          //linear approximation algorith used 
         
          //populate output array with measured values
          for(i=0; i < inputPixels; i++){
             output[(i%C)*5+(i/C)*5*outCol]=input[i];
          }
         
          //populate output array with derived values for sides of 6x6 box
           for(i=0; i < inputPixels; i++){
                //save output image offset
                j=(i%C)*5+(i/C)*5*outCol;
                
                //For each square of four calculate:
                
                //left and right side
                difference = input[i+C]-input[i];
                increment = difference/5;
                for(k=1; k < 5; k++){
                   output[j+outCol*k] = output[j+outCol*(k-1)]+increment;
                }
                
                //skip right side of original image
                if(i%C == C-1) continue;
                
                //top and bottom side
                difference = input[i+1]-input[i];
                increment = difference/5;
                for(k=1; k < 5; k++){
                   output[j+k] = output[(j+k)-1]+increment;
                }
           }
           
           //for all rows except the last and right side, fill in the square
           for(i=0; i < (inputPixels-C);i++){
           
                //skip right side of original image
                if(i%C == C-1) continue;
                
                //save output image offset
                j=(i%C)*5+(i/C)*5*outCol;
                
                //Top left to bottom right
                difference = input[i+C+1]-input[i];
                increment = difference/5;
                for(k=1; k < 5; k++){
                   output[j+outCol*k+k] = output[j+outCol*(k-1)+k-1]+increment;
                }
                
                //Top right to bottom left
                difference = input[i+C]-input[i+1];
                increment = difference/5;
                for(k=1; k < 5; k++){
                   output[j+outCol*k-k+5] = output[j+outCol*(k-1)-k+5+1]+increment;
                }
                
                //fill in the 8 gaps manually (replace this for generic solution)
                //top gap
                output[j+outCol+2]=output[j+2]+(output[j+2*outCol+2]-output[j+2])/2;
                output[j+outCol+3]=output[j+3]+(output[j+2*outCol+3]-output[j+3])/2;
                
                //left gap
                output[j+2*outCol+1]=output[j+2*outCol]+(output[j+2*outCol+2]-output[j+2*outCol])/2;
                output[j+3*outCol+1]=output[j+3*outCol]+(output[j+3*outCol+2]-output[j+3*outCol])/2;
                
                //right gap
                output[j+2*outCol+4]=output[j+2*outCol+3]+(output[j+2*outCol+5]-output[j+2*outCol+3])/2;
                output[j+3*outCol+4]=output[j+3*outCol+3]+(output[j+3*outCol+5]-output[j+3*outCol+3])/2;
                
                //bottom gap
                output[j+4*outCol+2]=output[j+3*outCol+2]+(output[j+5*outCol+2]-output[j+3*outCol+2])/2;
                output[j+4*outCol+3]=output[j+3*outCol+3]+(output[j+5*outCol+3]-output[j+3*outCol+3])/2;
          }
     }
     return;
}

void printInput(){
     int i=0, count=0;
     
     printf("\nInput Image\n");
     for(i=0; i<inputPixels; i++){
           if(count >= C){
                count = 0;
                printf("\n\n");
           }
           printf("%3.1f ", input[i]);
           count++;
           
     }
     
     printf("\n\n");
     return;
}

void printOutput(){
     int i=0, count=0;
     
     printf("\nOutput Image\n");
     for(i=0; i<outputPixels; i++){
           if(count >= outCol){
                count = 0;
                printf("\n\n\n");
           }
           printf("%3.1f ", output[i]);
           count++;
     }
     
     printf("\n\n");
     return;     
}




