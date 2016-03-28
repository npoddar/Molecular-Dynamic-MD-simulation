#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define totalTolueneAtoms 404595
#define true 1
#define false 2


int carbons[totalTolueneAtoms + 1];
int CT[totalTolueneAtoms + 1];
int CAofCT[totalTolueneAtoms + 1];


void carbonreader();
void printTest();
void bondReader();



int main(int argc, char *argv[]){

	carbonreader();
	printTest();
	bondReader();

}

void carbonreader(){

	FILE *carbonData = fopen("dipoleZ.4902000.nve", "rt");

	char line[2000000];
	
	for(int i = 1; i < (totalTolueneAtoms + 1); i++){
		carbons[i] = false;
		CT[i] = false;
	}

	while( (fgets(line, 2000000, carbonData) != NULL) ) {
		int id, type, mol;
		float charge, xcoor, ycoor, zcoor;		

		int scan = sscanf(line, "%d %d %d %f %f %f %f", &id, &mol, &type, &charge, &xcoor, &ycoor, &zcoor);

		if ( (scan == 7) ){
			carbons[id] = true;

			if(type == 2){
					CT[id] = true;
			}

		}//if case

	}//while loop

	fclose(carbonData);

}//end of carbonReader method



void printTest(){

	FILE *testOutput = fopen("testCarbon.txt", "w+");
	FILE *testOutput1 = fopen("testCT.txt", "w+");

	for(int i = 0; i < (totalTolueneAtoms + 1); i++){
		fprintf(testOutput, "%d %d\n", i, carbons[i]);
		fprintf(testOutput1, "%d %d\n", i, CT[i]);
	}

	fclose(testOutput);
	fclose(testOutput1);

}


void bondReader(){
	FILE *bond = fopen("bonds.txt", "rt");
	FILE *headNeck = fopen("CT_CA.atoms", "w+");

	char line[2000000];

	int CT_CA = 0;

	while( (fgets(line, 2000000, bond) != NULL) )
	{
		int bondNo, bondType, atomid1, atomid2;

		sscanf(line, "%d %d %d %d", &bondNo, &bondType, &atomid1, &atomid2);

		if( (CT[atomid1] == true) ){

			if(carbons[atomid2] == true){
				fprintf(headNeck, "%d %d\n", atomid1, atomid2);
				CT_CA++;

			}
		}//if statement
		else if( (CT[atomid2] == true) ){
			if(carbons[atomid1] == true){
				fprintf(headNeck, "%d %d\n", atomid2, atomid1);
				CT_CA++;
			}
		}
			
	}//end of while loop

	
	printf("No of CT_CA bonds %d\n", CT_CA);
}//end of procedure

