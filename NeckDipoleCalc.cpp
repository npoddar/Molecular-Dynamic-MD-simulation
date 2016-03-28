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
int timestepValue;

void carbonStoreZero();
void readCarbonData(char *);
void printCarbonData();
void dipoleCalc(char *);
void submain(char *, char *);



int main(int argc, char *argv[]){

	FILE *fileInput = fopen(argv[1], "rt");
	FILE *output = fopen(argv[2], "w+");
	fclose(output);

	char fileLine[2000000];

	while( (fgets(fileLine, 2000000, fileInput) != NULL) ){
		char *fileName;

		fileName = (char*) malloc(2000000);

		sscanf(fileLine, "%s", fileName);

		submain(fileName, argv[2]);
	}//end of while loop

	fclose(fileInput);
	
}


void submain(char *fileName, char *output){

	carbonStoreZero();
	readCarbonData(fileName);
	printCarbonData();
	dipoleCalc(output);

}


void readCarbonData(char *inputfileName){
	FILE *input = fopen(inputfileName, "rt");

	printf("Reading file %s\n", inputfileName);
	
	char line[2000000];
	int timestep = 0;
	timestepValue = 0;

	while( (fgets(line, 2000000, input) != NULL) ){
		timestep++;

		if(timestep == 2){
			sscanf(line, "%d", &timestepValue);	
		}

		int id1, type1, mol1;
		float charge1, xcoor1, ycoor1, zcoor1;		

		int scan = sscanf(line, "%d %d %d %f %f %f %f", &id1, &mol1, &type1, &charge1, &xcoor1, &ycoor1, &zcoor1);

		if( (scan == 7) ){
			

			allCarbonStoreInt[id1][molecule] = mol1;
			allCarbonStoreInt[id1][type] = type1;

			allCarbonStoreFloat[id1][charge] = charge1;
			allCarbonStoreFloat[id1][z] = zcoor1;

		}//end of if statement//

	}//end of while loop

	fclose(input);
}

void printCarbonData(){
	
	FILE *test = fopen("CarbonDataTest.txt", "a+");

	for(int i = 1; i < (totalTolueneAtoms + 1); i++){
		fprintf(test, "%d %d %d %f %f\n", i, allCarbonStoreInt[i][molecule], allCarbonStoreInt[i][type], allCarbonStoreFloat[i][charge], allCarbonStoreFloat[i][z]);
	}

	fclose(test);
}

void carbonStoreZero(){

	for(int i = 0; i < (totalTolueneAtoms + 1); i++){
		allCarbonStoreInt[i][molecule] = 0;
		allCarbonStoreInt[i][type] = 0;
	
		allCarbonStoreFloat[i][charge] = 0.0;
		allCarbonStoreFloat[i][z] = 0.0;

	}//for loop ends

	timestepValue = 0;
}

void dipoleCalc(char *output){

	FILE *CT_CA = fopen("CT_CA.atoms", "rt");
	FILE *outputFile = fopen(output, "a+");
	
	char line[2000000];

	float sum = 0.0;
	int N = 0;

	while( (fgets(line, 2000000, CT_CA) !=NULL)) {
		
		N++;
		int CT, CA;

		sscanf(line, "%d %d", &CT, &CA);
		
		sum = sum + (allCarbonStoreFloat[CT][z] - allCarbonStoreFloat[CA][z]);

	}

	float avgOrientation = sum / N;

	float dipole = avgOrientation *  0.118;

	printf("The number of molecules = %d\n", N);
	printf("Dipole Moment =%f\n", dipole);

	fprintf(outputFile, "Timestep = %d Dipole Moment = %d\n", timestepValue, dipole);

	fclose(CT_CA);
	fclose(outputFile);

}