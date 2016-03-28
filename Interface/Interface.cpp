#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define delta 1.0
#define noAtoms 160320 

void interfaceCalc(char *);
int num[1000];

int main(){

	//the movieFile is an xyz file//
	//it contains 425250 atom values
	//type xcoor ycoor zcoor//
	char *movieFile;
	movieFile = (char*) malloc(2000);

	printf("Enter the xyz file that contains atom coordinates: ");
	scanf(" %s", movieFile);

	interfaceCalc(movieFile);

}

void interfaceCalc(char *xyzFile){
	
	FILE *input = fopen(xyzFile, "rt");
	FILE *output = fopen("frame1.txt", "w+");

	for(int jloop=0;jloop<1000;jloop++)
	{
		num[jloop]=0;
	}

	char line[200000];
	
	int atomCount = 0;

	while( (fgets(line, 200000, input) != NULL ) ) {
		int type;
		float xcoor;
		float ycoor;
		float zcoor;

		int scan = sscanf(line, "%d %f %f %f", &type, &xcoor, &ycoor, &zcoor);

		if(scan == 4){
			atomCount++;
			if(type < 5){
				int j=(int) (zcoor/delta);
				num[j]=num[j] + 1;
			}//if type < 5

		}// if scan = 4

		if(atomCount == noAtoms){
			break;
		}

	}//while loop

	fprintf(output, "zcoor atoms\n");
	for(int jprint=0;jprint<1000;jprint++){
		fprintf(output, "%f %d\n",jprint*delta+delta/2, num[jprint]);
	}

}