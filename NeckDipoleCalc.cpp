#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//////////////////////////////////////////////
//This program reads in the CT_CA atom ids from the text file generated by NeckDipole, and computes NeckDipole of 
//individual molecules and sums over all molecule
//////////////////////////////////////////////


#define totalTolueneAtoms 404595

#define molecule 0
#define type 1
#define charge 0
#define z 1

int allCarbonStoreInt[totalTolueneAtoms + 1][2];
float allCarbonStoreFloat[totalTolueneAtoms + 1][2];

void carbonStoreZero();

int main(int argc, char *argv[]){




}



void carbonStoreZero(){

	for(int i = 0; i < (totalTolueneAtoms + 1); i++){
		allCarbonStoreInt[i][molecule] = 0;
		allCarbonStoreInt[i][type] = 0;
	
		allCarbonStoreFloat[i][charge] = 0.0;
		allCarbonStoreFloat[i][z] = 0.0;

	}//for loop ends


}