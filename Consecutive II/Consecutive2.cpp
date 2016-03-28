//Dump file can be sorted by atom id or molecule id

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//highest value of atomid
#define atomsBeforeDel 500000 

#define restartAtoms 160290
#define restartBonds 159750 
#define restartAngles 262512
#define restartDihedrals 333000

void consecutiveAtoms();
void bonds();
void angles();
void dihedrals();

int atomId[atomsBeforeDel+1];

void intro(char *, char *);
void consecutiveAtoms(char *, char *);
void bonds(char *, char *);	
void angles(char *, char *);
void dihedrals(char *, char *);

int main(){

	char *dumpAtoms;
	dumpAtoms = (char*) malloc(2000);

	char *restartFile;
	restartFile = (char*) malloc(2000);

	char *newRestartFile;
	newRestartFile = (char*) malloc(2000);

	dumpAtoms = "atom.restart.moveLiquid";
	restartFile = "restart.250000.data";
	newRestartFile = "restart.250000.data.consecutive";
	
	intro(restartFile, newRestartFile);
	consecutiveAtoms(dumpAtoms, newRestartFile);
	bonds(restartFile, newRestartFile);	
	angles(restartFile, newRestartFile);
	dihedrals(restartFile, newRestartFile);

}

//print everything until atoms section start//
void intro(char *restartFile, char *newRestartFile){
	
	//printf("Entered intro");
	
	FILE *input = fopen(restartFile, "rt");
	FILE *output = fopen(newRestartFile, "w+");
	
	char line[2000000];

	//printf("Entered intro1");
	
	while( (fgets(line, 2000000, input) != NULL ) ){

		char *str;
		
		str = (char*) malloc(2000000);

		int scan = sscanf(line, "%s", str);

		//if "Atoms" word comes up, leave while loop
		if((strcmp(str, "Atoms") == 0)){
			//leave while loop//
			free(str);
			break;
		}

		fprintf(output, "%s", line);

		free(str);
	}
	
	fclose(input);
	fclose(output);
	
}

void consecutiveAtoms(char *dumpAtoms, char *newRestartFile){
	//Read atoms from dump file, and store its consecutive id number instead
	//index of array atomId[] is its actual id. The stored int is its consecutive id.

	FILE *input = fopen(dumpAtoms, "rt");
	FILE *output = fopen(newRestartFile, "a+");

	fprintf(output, "\n\n\nAtoms\n\n");
	
	char line[2000000];
	int index = 1;

	while( (fgets(line, 2000000, input) != NULL ) ){

		int id, molid, type;
		float charge, xcoor, ycoor, zcoor;
		int imagex, imagey, imagez;

		int scan = sscanf(line, "%d %d %d %f %f %f %f %d %d %d", &id, &molid, &type, &charge, &xcoor, &ycoor, &zcoor, &imagex, &imagey, &imagez);
		
		if(scan == 10){
			atomId[id] = index;
			fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", index, molid, type, charge, xcoor, ycoor, zcoor, imagex, imagey, imagez);
			index++;
		}


	}

	printf("Value of index is %d\n", index);

	fclose(input);
	fclose(output);

}

void bonds(char *oldRestartFile, char *newRestartFile){

	FILE *input = fopen(oldRestartFile, "rt");
	FILE *output = fopen(newRestartFile, "a+");

	fprintf(output, "\n\nBonds\n\n");

	bool bondEnd = false;
	int section = 0;
	int oldBondCount = 0;

	char line[2000000];

	while( (fgets(line, 200000, input) != NULL ) && (oldBondCount < restartBonds) ) {
		char *str;
		
		str = (char*) malloc(2000000);

		int scan = sscanf(line, "%s", str);

		if(strcmp(str, "Bonds") == 0){
			printf("Bonds section reached\n");
			bondEnd = true;
		}
		
		free(str);

		if(bondEnd){
			section++;
		}

		if(section > 2){

			
			oldBondCount++;
			
			int bondId;
			int bondType;
			int atomid1;
			int atomid2;
			

			int scan = sscanf(line, "%d %d %d %d", &bondId, &bondType, &atomid1, &atomid2);

			fprintf(output, "%d %d %d %d\n", bondId, bondType, atomId[atomid1], atomId[atomid2]);

		}

		

	}//end of while loop


	fclose(input);
	fclose(output);

}

void angles(char *oldRestartFile, char *newRestartFile){

	FILE *input = fopen(oldRestartFile, "rt");
	FILE *output = fopen(newRestartFile, "a+");

	fprintf(output, "\n\nAngles\n\n");

	char line[2000000];

	bool bondEnd = false;
	int section = 0;
	int oldBondCount = 0;

	while( (fgets(line, 200000, input) != NULL ) && (oldBondCount < restartAngles) ) {
		char *str;
		
		str = (char*) malloc(2000000);

		int scan = sscanf(line, "%s", str);

		if(strcmp(str, "Angles") == 0){
			printf("Angles section reached\n");
			bondEnd = true;
		}
		
		free(str);

		if(bondEnd){
			section++;
		}

		if(section > 2){

			oldBondCount++;
			
			int angleId;
			int angleType;
			int atomid1;
			int atomid2;
			int atomid3;
			
			int scan = sscanf(line, "%d %d %d %d %d", &angleId, &angleType, &atomid1, &atomid2, &atomid3);

			fprintf(output, "%d %d %d %d %d\n", angleId, angleType, atomId[atomid1], atomId[atomid2], atomId[atomid3]);

		}

	}//end of while loop

	fclose(input);
	fclose(output);

}

void dihedrals(char *oldRestartFile, char *newRestartFile){

	FILE *input = fopen(oldRestartFile, "rt");
	FILE *output = fopen(newRestartFile, "a+");

	fprintf(output, "\n\nDihedrals\n\n");

	char line[2000000];
	bool bondEnd = false;
	int section = 0;
	int oldBondCount = 0;

	while( (fgets(line, 200000, input) != NULL ) && (oldBondCount < restartDihedrals) ) {
		char *str;
		
		str = (char*) malloc(2000000);

		int scan = sscanf(line, "%s", str);

		if(strcmp(str, "Dihedrals") == 0){
			printf("Dihedrals section reached\n");
			bondEnd = true;
		}
		
		free(str);

		if(bondEnd){
			section++;
		}

		if(section > 2){

			oldBondCount++;
			
			int angleid, angletype, id1, id2, id3, id4;
			
			sscanf(line, "%d %d %d %d %d %d", &angleid, &angletype, &id1, &id2, &id3, &id4);

			fprintf(output, "%d %d %d %d %d %d\n", angleid, angletype, atomId[id1], atomId[id2], atomId[id3], atomId[id4]);

		}

		
	}//end of while loop


	fclose(input);
	fclose(output);

}