//This program reads a file that contains molecule ids.
//Then dipole is calculated for toluene molecules by reading toluene data file and finding
//molecules that were read from the first file.
//Dipole is calculated for all the molecules and output to another file

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define noAtoms 404595

int mol[30000];
int molecules;
float atom[noAtoms+1];

void getMolecule(char *);
void printMolecules();
void atomCharge();
void printAtom();
void dipoleCalc(char *, char *);


int main(){

	atomCharge();

	int molecules = 0;

	for(int i = 0; i < 30000; i++){
		mol[i] = 0;
	}

	char *moleculeFile;
	moleculeFile = (char*) malloc(2000000);

	char *TolueneFile;
	TolueneFile = (char*) malloc(2000000);

	char *outputFile;
	outputFile = (char*) malloc(2000000);

	printf("Enter the file that contains the molecule ids : ");
	scanf(" %s", moleculeFile);

	printf("Enter the file that contains Toluene coordinates : ");
	scanf(" %s", TolueneFile);

	printf("Enter the file to output dipole values : ");
	scanf(" %s", outputFile);

	getMolecule(moleculeFile);
	printMolecules();

	dipoleCalc(TolueneFile, outputFile);

}

void atomCharge(){

	FILE *input = fopen("coordinates.5600000.toluene.charge.txt", "rt");

	char line[2000000];
	int i = 0;

	while( (fgets(line, 2000000, input) != NULL) ){
		int atomid;
		float charge;

		sscanf(line, "%d %*d %*d %f %*f %*f %*f", &atomid, &charge);

		atom[atomid] = charge;
		i++;
	}
	
	fclose(input);
	printAtom();
	printf("No of atoms %d\n", i);

}

void printAtom(){
	FILE *output = fopen("Atom.charge", "w+");
	
	for(int i = 1; i < (noAtoms + 1); i++){
		
		fprintf(output, "%d %f\n", i, atom[i]);
	}
	
	fclose(output);
}

void getMolecule(char *moleculeFile1){

	char *molFile;
	molFile = (char*) malloc(2000000);

	molFile = moleculeFile1;

	printf("Molecule file name %s \n", moleculeFile1);
	printf("Molecule file name %s \n", molFile);

	FILE *input = fopen(molFile, "rt");
	char line[2000000];
	int i = 0;

	while( (fgets(line, 2000000, input) != NULL) ){
		int moleculeid;

		sscanf(line, "%d", &moleculeid);

		mol[i] = moleculeid;

		i++;
	}//end of while loop

	fclose(input);
	molecules = i;
	printf("The number of molecules = %d\n", molecules);

}

void printMolecules(){
	FILE *output = fopen("MoleculesOutput.test", "w+");

	for(int i = 0; i < molecules; i++){
		fprintf(output, "%d\n", mol[i]);
	}

	fclose(output);
}

void dipoleCalc(char *tolueneInput, char *dipoleOutput){
	
	printf("Toluene file name %s\n", tolueneInput);
	printf("Dipole output file %s\n", dipoleOutput);
	
	//"atom[id]" contains charges for each atom
	//"mol[i]" contains array of molecules in the region
	//"molecules" is no of molecules in region
	//"noAtoms" is no of atoms in toluene starting from 1

	FILE *input = fopen(tolueneInput, "rt");
	FILE *output = fopen(dipoleOutput, "a+");

	char line[2000000];
	int timestep = 0;
	int timestepValue = 0;
	int atomCount = 0;
	int moleculeFile = 0;
	float dipoleSum = 0.0;
	int onefive = 0;

	while( (fgets(line, 2000000, input) != NULL) ) {
		timestep++;

		if(timestep == 2) {
			sscanf(line, "%d", &timestepValue);
		}

		int atomid, moleculeid;
		float xcoor, ycoor, zcoor;

		int scan = sscanf(line, "%d %d %f %f %f", &atomid, &moleculeid, &xcoor, &ycoor, &zcoor);
	
		if( (scan == 5) ){
			if(moleculeid == mol[moleculeFile]){
				dipoleSum = dipoleSum + (atom[atomid] * zcoor);
				onefive++;
			}
			if(onefive == 15){
				moleculeFile++;
				onefive = 0;
			}

			atomCount++;

		}//end of if statement "scan = 5"

		if(atomCount == noAtoms)
		{//read all atoms for the timestep//
			if(moleculeFile != molecules){
				printf("No of files in region %d, but toluene file has only %d\n", molecules, moleculeFile);
				while(true)
				{
					;
				}
			}
		
			dipoleSum = dipoleSum / moleculeFile;
			fprintf(output, "Timestep = %d, Dipole value = %f\n", timestepValue, dipoleSum);

			//setting everything to zero
			moleculeFile = 0;
			timestep = 0;
			timestepValue = 0;
			dipoleSum = 0;
			atomCount = 0;
			onefive = 0;

		}//end of if statement (atomCount == noAtoms)


	}//end of while loop


}//end of method