//This program moves molecules in the lower half of the toluene block up on top of the top half//
//Needed dump file containing atom coordinates of the current run//
//Needed restart file which will be altered only in the atoms section//


#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//block to move up//
#define zlow -1
#define zhigh 188
#define restartAtoms 160320 
#define zshift 214 /////////////////////////////////////////////////////////////////////

int mol[500000];
int moldist[200000];
int molHash[1000000];


void molecules(char *);
void createRestart(char *, char *);
void intro(char *, char *);
void moleculeIndex();
void writeAtoms(char *, char *);
void copyRest(char *, char *);



int main(){
	
	char *dumpAtoms;
	dumpAtoms = (char*) malloc(2000);

	char *restartFile;
	restartFile = (char*) malloc(2000);

	char *newRestartFile;
	newRestartFile = (char*) malloc(2000);

	printf("Enter the dump file that contains atom information sorted by moleculeId: ");
	//this file contains atom coordinates as in restart file.
	scanf(" %s", dumpAtoms);

	printf("Enter the restart file that contain atom configuration: ");
	scanf(" %s", restartFile);

	printf("Enter the name of the new restart file: ");
	scanf(" %s", newRestartFile);

	molecules(dumpAtoms);

	createRestart(restartFile, newRestartFile);



}


//go over a file containing atom coordinates, and creates an array of molecules in the lower half of the liquid
void molecules(char *atomfile){

	FILE *input = fopen(atomfile, "rt");
	FILE *output = fopen("output.molecules.txt", "w+");
	FILE *outputd = fopen("output.molecules_d.txt", "w+");
	char line[200000];
	int molct = 0;

	while( (fgets(line, 200000, input) != NULL ) ) {
		int atomid;
		int moleculeid;
		int type;
		float charge;
		float xcoor;
		float ycoor;
		float zcoor;
		int imagex, imagey, imagez;

		int scan = sscanf(line, "%d %d %d %f %f %f %f %d %d %d", &atomid, &moleculeid, &type, &charge, &xcoor, &ycoor, &zcoor, &imagex, &imagey, &imagez);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if(scan == 10){
			if( ((zcoor > zlow) && (zcoor < zhigh)) && (type < 5) ) {	///////////////////////////////////////////////////////////////////////////////
				//if in this zone, save molecule ids, and shift liquid up//
				mol[molct] = moleculeid;
				molct++;
			}
		}

	}//end of while loop

	moldist[0] = mol[0];
	int dist = 0;

	for(int i = 0; i < molct; i++) {
		if(moldist[dist] != mol[i]){
			dist++;
			moldist[dist] = mol[i];
		}
	}

	for(int i = 0; i < molct; i++){
		fprintf(output, "%d\n", mol[i]);
	}

	for(int i = 0; i < dist+1; i++){
		fprintf(outputd, "%d\n", moldist[i]);
	}

	printf("No of atom in lower half of toluene %d\n", molct);
	printf("No of molecules %d\n", dist);

	//delMolecules = dist;

	fclose(input);
	fclose(output);
	fclose(outputd);

}

//create the restart file "newRestartFile" from "restartFile"
void createRestart(char *restartFile, char *newRestartFile){

	intro(restartFile, newRestartFile);
	moleculeIndex();
	writeAtoms(restartFile, newRestartFile);
	copyRest(restartFile, newRestartFile);
	
	/*
	writeAtoms(restartFile, newRestartFile);
	writeVelocities(restartFile, newRestartFile);
	writeBonds(restartFile, newRestartFile);
	writeAngles(restartFile, newRestartFile);
	writeDihedrals(restartFile, newRestartFile);
	*/

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

//create index to molecules to shift. if molHash[index] = 1; shift the molecule up.//
void moleculeIndex(){

	for(int i = 0; i < 1000000; i++){
		molHash[i] = 0;
	}

	for(int i = 0; i < 200000; i++){
		int mol = moldist[i];
		molHash[mol] = 1;
	}

}


//Open restart file named "restartFile".
//Go to the atom section in the file
//Copy one line to a new file if it fits a certain criteria
void writeAtoms(char *restartFile, char *newRestartFile){

	FILE *restart = fopen(restartFile, "rt");
	FILE *output = fopen(newRestartFile, "a+");
	char line[200000];
	bool atomEnd = false;
	int section = 0;
	int atomCount = 0;
	int newAtomCount = 0;
		
	while( (fgets(line, 200000, restart) != NULL ) && (atomCount < restartAtoms) ) {
		char *str;
		
		str = (char*) malloc(2000000);

		int scan = sscanf(line, "%s", str);

		if(strcmp(str, "Atoms") == 0){
			printf("Atom section reached\n");
			atomEnd = true;
		}
		
		free(str);

		if(atomEnd){
			section++;
		}

		if(section > 2){

			if(section == 3){
				fprintf(output, "\n\nAtoms\n\n");
			}

			atomCount++;
			
			int atomid;
			int molid;
			int atomType;
			float charge;
			float xcoor, ycoor, zcoor;
			int imagex, imagey, imagez;


			int scan = sscanf(line, "%d %d %d %f %f %f %f %d %d %d", &atomid, &molid, &atomType, &charge, &xcoor, &ycoor, &zcoor, &imagex, &imagey, &imagez);

			if(molHash[molid] == 0){
				//print it
				fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", atomid, molid, atomType, charge, xcoor, ycoor, zcoor, imagex, imagey, imagez);
				newAtomCount++;
			}
			else{
				fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", atomid, molid, atomType, charge, xcoor, ycoor, (zcoor + zshift), imagex, imagey, imagez);
				newAtomCount++;
			}

		}

		//printf(line);

		//printf("Working\n");

	}//end of while loop

	printf("Entered writeAtoms\n");
	printf("New count of atoms %d\n", newAtomCount);

	fclose(restart);
	fclose(output);
}

void copyRest(char *restartFile, char *newRestartFile){


	FILE *input = fopen(restartFile, "rt");
	FILE *output = fopen(newRestartFile, "a+");

	fprintf(output, "\n\n\n");
	
	char line[2000000];

	//printf("Entered intro1");

	bool rest = false;
	
	while( (fgets(line, 2000000, input) != NULL ) ){

		char *str;
		
		str = (char*) malloc(2000000);

		int scan = sscanf(line, "%s", str);

		//if "Atoms" word comes up, leave while loop
		if((strcmp(str, "Velocities") == 0)){
			//leave while loop//
			rest = true;
		}

		if(rest){
			fprintf(output, "%s", line);
		}

		free(str);
	}
	
	fclose(input);
	fclose(output);


}