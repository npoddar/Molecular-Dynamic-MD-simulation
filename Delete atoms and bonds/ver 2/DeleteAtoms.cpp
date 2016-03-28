//Needed : dump file that contains atom coordinates as in restart file sorted by molecule-id.
//Find list of molecules to delete

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//inclusion zone//
#define xlow 150
#define xhigh 360
#define ylow 150
#define yhigh 360
#define zlow 150
#define zhigh 360

//center of new box
float xcen;
float ycen;
float zcen;
int radiusdel;

#define del 1
#define keep 0

//restart atoms and bonds//
#define restartAtoms 3240000
#define restartBonds 3240000 
#define restartAngles 5184000
#define restartDihedrals 6480000

int mol[3240000];
int moldist[3240000];
int delMolecules; //no of molecules you want to delete//
int atom[3240000];
int delAtoms;
int atomFlag[3240000];//atom index = atom id. If 1, atom to be deleted. If zero, to be kept
int newAtomCount1;

void molecules(char *);
void getDelAtoms(char *);
void printDelAtoms();
void createRestart(char *, char *);
void atomIndex();
void writeAtoms(char *, char *);
void writeBonds(char *, char *);
void writeAngles(char *, char *);
void writeDihedrals(char *, char *);
void intro(char *, char *);
void writeVelocities(char *, char *);

int main(){

	newAtomCount1 = 0;

	xcen = 255.0;
	ycen = 255.0;
	zcen = 255.0;

	radiusdel = 0;

	char *dumpAtoms;
	dumpAtoms = (char*) malloc(2000);

	char *restartFile;
	restartFile = (char*) malloc(2000);

	char *newRestartFile;
	newRestartFile = (char*) malloc(2000);

	//printf("Enter the dump file that contains atom information sorted by moleculeId: ");
	//this file contains atom coordinates as in restart file.
	//scanf(" %s", dumpAtoms);

	//printf("Enter the restart file that contain atom configuration: ");
	//scanf(" %s", restartFile);

	//printf("Enter the name of the new restart file: ");
	//scanf(" %s", newRestartFile);

	dumpAtoms = "atom.restart.0";
	restartFile = "restart.replicate.data";
	newRestartFile = "restart.toluene.21nm";

	molecules(dumpAtoms);
	printf("Value of delMolecules variable is %d\n", delMolecules);

	delAtoms = (delMolecules + 1) * 15;

	getDelAtoms(dumpAtoms);

	//printDelAtoms();

	createRestart(restartFile, newRestartFile);

	//free(dumpAtoms);
	//free(restartFile);
	//free(newRestartFile);

}



//go over a file containing atom coordinates, and creates an array of molecules to delete
void molecules(char *atomFile){
	FILE *input = fopen(atomFile, "rt");
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
		float radius;

		int scan = sscanf(line, "%d %d %d %f %f %f %f %d %d %d", &atomid, &moleculeid, &type, &charge, &xcoor, &ycoor, &zcoor, &imagex, &imagey, &imagez);

		if(scan == 10){
			if( ((xcoor < xlow)  || (xcoor > xhigh) ) || ( (ycoor < ylow)  || (ycoor > yhigh) ) || ( (zcoor < zlow) || (zcoor > zhigh) ) ) {
				mol[moleculeid] = del;
				//molct++;
			}	//first delete list

			//carving out a sphere//
			float xlen = xcen - xcoor;
			float ylen = ycen - ycoor;
			float zlen = zcen - zcoor;

			radius = (xlen*xlen) + (ylen*ylen) + (zlen*zlen);
			radius = sqrt(radius);

			if(radius < 53.0){
				mol[moleculeid] = del;
				radiusdel++;
			}		
			////////////////////////////////
		}//if (scan == 10)


	}//end of while loop
	
	/*
	moldist[0] = mol[0];
	int dist = 0;

	for(int i = 0; i < molct; i++) {
		if(moldist[dist] != mol[i]){
			dist++;
			moldist[dist] = mol[i];
		}

	}
	*/

	/*
	for(int i = 0; i < molct; i++){
		fprintf(output, "%d\n", mol[i]);
	}

	for(int i = 0; i < dist+1; i++){
		fprintf(outputd, "%d\n", moldist[i]);
	}
	*/

	/*printf("No of atom in block region %d\n", molct);
	printf("No of molecules %d\n", dist);

	delMolecules = dist;
	*/

	printf("Number of molecules within inner radius %d\n", radiusdel);
	fclose(input);
	fclose(output);
	fclose(outputd);

}

//create an array of atoms which will be deleted
//method changed//
void getDelAtoms(char *atomFile){

	FILE *input = fopen(atomFile, "rt");
	char line[200000];
	int molct = 0; //index for molecule array
	int atomct = 0; //index for atom array
	bool readAtoms = false;
	int singlemol = 0;	//no of atoms in 1 toluene molecule

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

		if(scan == 10){
			
			//set atom flag to del if to be deleted//
			if(mol[moleculeid] == del){
				atom[atomid] = del;
			}
			else{
				atom[atomid] = keep;
				newAtomCount1++;
			}

			/////////////////////////////////
			
			//if moleculeid == moldist[top], read next 15 atoms. increase moldist count by 1. try to match moldist[i] with moleculeid. Repeat

			/*
			if( (moldist[molct] == moleculeid) && (!readAtoms) ){
				readAtoms = true;
				molct++;		
			}

			if(readAtoms){
				atom[atomct] = atomid;
				atomct++;
				singlemol++;
				if(singlemol == 15){
					readAtoms = false;
					singlemol = 0;
				}
			}
			*/




		}//end of scan == 10


	}//end while loop


	/*
	if(molct != delMolecules){
		printf("The number of molecules parsed through %d\n", molct);
	}
	
	printf("The number of atoms to delete %d\n", atomct);
	*/

	fclose(input);

	printf("New count of atoms %d\n" , newAtomCount1);

}

//print from an array the atoms that will be deleted

void printDelAtoms(){

	FILE *output = fopen("Atoms.del", "w+");

	for(int i = 0; i < delAtoms; i++){
		fprintf(output, "%d\n", atom[i]);
	}

	fclose(output);
}

//create an array of all atoms 
void atomIndex(){

	for(int i = 0; i < 500000; i++){
		atomFlag[i] = 0;
	}

	for(int i = 0; i < delAtoms; i++){
		int atomId = atom[i];
		atomFlag[atomId] = 1;
	}

	
}


//create the restart file "newRestartFile" from "restartFile"
void createRestart(char *restartFile, char *newRestartFile){

	//atomIndex();
	intro(restartFile, newRestartFile);
	writeAtoms(restartFile, newRestartFile);
	writeVelocities(restartFile, newRestartFile);
	writeBonds(restartFile, newRestartFile);
	writeAngles(restartFile, newRestartFile);
	writeDihedrals(restartFile, newRestartFile);

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

			int scan = sscanf(line, "%d %*d %*d %*f %*f %*f %*f %*d %*d %*d", &atomid);

			if(atom[atomid] == keep){
				//print it
				fprintf(output, line);
				newAtomCount++;
			}
			else{
				;
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


//Open restart file named "restartFile".
//Go to the Bonds section in the file
//Copy one line to the new file if it fits a certain criteria

void writeBonds(char *oldRestartFile, char *newRestartFile){

	FILE *restart = fopen(oldRestartFile, "rt");
	FILE *output = fopen(newRestartFile, "a+");
	char line[200000];
	
	bool bondEnd = false;
	int section = 0;
	int oldBondCount = 0;
	int newBondCount = 0;
	
	printf("Entered writeBonds\n");
		
	while( (fgets(line, 200000, restart) != NULL ) && (oldBondCount < restartBonds) ) {
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

			if(section == 3){
				fprintf(output, "\n\nBonds\n\n");
			}

			oldBondCount++;
			
			int bondId;
			int bondType;
			int atomid1;
			int atomid2;
			

			int scan = sscanf(line, "%d %d %d %d", &bondId, &bondType, &atomid1, &atomid2);

			if( (atom[atomid1] == keep) && (atom[atomid2] == keep) )  {
				//print it
				newBondCount++;
				fprintf(output, "%d %d %d %d\n", newBondCount, bondType, atomid1, atomid2);
			}
			else{
				;
			}

		}

		//printf(line);

		//printf("Working\n");

	}//end of while loop

	printf("New count of bonds %d\n", newBondCount);

	fclose(restart);
	fclose(output);
}


//Open restart file named "restartFile".
//Go to the Angles section in the file
//Copy one line to the new file if it fits a certain criteria

void writeAngles(char *oldRestartFile, char *newRestartFile){

	FILE *restart = fopen(oldRestartFile, "rt");
	FILE *output = fopen(newRestartFile, "a+");
	char line[200000];
	
	bool bondEnd = false;
	int section = 0;
	int oldBondCount = 0;
	int newBondCount = 0;
	
	printf("Entered writeAngles\n");
		
	while( (fgets(line, 200000, restart) != NULL ) && (oldBondCount < restartAngles) ) {
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

			if(section == 3){
				fprintf(output, "\n\nAngles\n\n");
			}

			oldBondCount++;
			
			int bondId;
			int bondType;
			int atomid1;
			int atomid2;
			int atomid3;
			
			int scan = sscanf(line, "%d %d %d %d %d", &bondId, &bondType, &atomid1, &atomid2, &atomid3);

			if( (atom[atomid1] == 0) && (atom[atomid2] == 0) && (atom[atomid3] == 0) )  {
				//print it
				newBondCount++;
				fprintf(output, "%d %d %d %d %d\n", newBondCount, bondType, atomid1, atomid2, atomid3);
			}
			else{
				;
			}

		}

		//printf(line);

		//printf("Working\n");

	}//end of while loop

	printf("New count of angles %d\n", newBondCount);

	fclose(restart);
	fclose(output);
}


//Open restart file named "restartFile".
//Go to the Dihedrals section in the file
//Copy one line to the new file if it fits a certain criteria

void writeDihedrals(char *oldRestartFile, char *newRestartFile){

	FILE *restart = fopen(oldRestartFile, "rt");
	FILE *output = fopen(newRestartFile, "a+");
	char line[200000];
	
	bool bondEnd = false;
	int section = 0;
	int oldBondCount = 0;
	int newBondCount = 0;
	
	printf("Entered writeDihedrals\n");
		
	while( (fgets(line, 200000, restart) != NULL ) && (oldBondCount < restartDihedrals) ) {
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

			if(section == 3){
				fprintf(output, "\n\nDihedrals\n\n");
			}

			oldBondCount++;
			
			int bondId;
			int bondType;
			int atomid1;
			int atomid2;
			int atomid3;
			int atomid4;
			
			int scan = sscanf(line, "%d %d %d %d %d %d", &bondId, &bondType, &atomid1, &atomid2, &atomid3, &atomid4);

			if( (atom[atomid1] == 0) && (atom[atomid2] == 0) && (atom[atomid3] == 0) && (atom[atomid4] == 0) )  {
				//print it
				newBondCount++;
				fprintf(output, "%d %d %d %d %d %d\n", newBondCount, bondType, atomid1, atomid2, atomid3, atomid4);
			}
			else{
				;
			}

		}

		//printf(line);

		//printf("Working\n");

	}//end of while loop

	printf("New count of dihedrals %d\n", newBondCount);

	fclose(restart);
	fclose(output);
}

//Open restart file named "restartFile".
//Go to the velocity section in the file
//Copy one line to a new file if it fits a certain criteria
void writeVelocities(char *restartFile, char *newRestartFile){

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

		if(strcmp(str, "Velocities") == 0){
			printf("Velocities section reached\n");
			atomEnd = true;
		}
		
		free(str);

		if(atomEnd){
			section++;
		}

		if(section > 2){

			if(section == 3){
				fprintf(output, "\n\nVelocities\n\n");
			}

			atomCount++;
			
			int atomid;
			float xvel, yvel, zvel;
			
			int scan = sscanf(line, "%d %f %f %f", &atomid, &xvel, &yvel, &zvel);

			if(atom[atomid] == 0){
				//print it
				fprintf(output, "%d %f %f %f \n", atomid, xvel, yvel, zvel);
				newAtomCount++;
			}
			else{
				;
			}

		}

		//printf(line);

		//printf("Working\n");

	}//end of while loop

	printf("Entered writeVelocities\n");
	printf("New count of velocities %d\n", newAtomCount);

	fclose(restart);
	fclose(output);
}
