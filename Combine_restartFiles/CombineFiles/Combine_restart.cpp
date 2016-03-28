//Needed : Thiol restart data file, and Toluene restart data file.
//Combine files into one restart file//

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

void intro(char *, char *);
void atoms(char *, char *, char *);
void velocities(char *, char *, char *);
void bonds(char *, char *, char *);
void angles(char *, char *, char *);
void dihedrals(char *, char *, char *);

#define thiol 1
#define toluene 0
#define highestToluene 5000000 

int atom[10000000];

float xcenToluene;
float ycenToluene;
float zcenToluene;

float xcenThiol;
float ycenThiol;
float zcenThiol;

int main(){

	xcenToluene = 255.0;
	ycenToluene = 255.0;
	zcenToluene = 255.0;

	xcenThiol = 161.98037;
	ycenThiol = 158.58858;
	zcenThiol = 262.8053;

	char *Toluene_restartFile;
	Toluene_restartFile = (char*) malloc(2000);

	char *Thiol_restartFile;
	Thiol_restartFile = (char*) malloc(2000);

	char *newRestartCombine;
	newRestartCombine = (char*) malloc(2000);

	Toluene_restartFile = "restart.toluene.21nm"; //Toluene restart file
	Thiol_restartFile = "restart.thiol.data";	// Thiol restart file
	newRestartCombine = "restart.combine.data";	//New combined restart file
	
	//char * oldRestartFile = "restartCombine.old.data";
	
	intro(Thiol_restartFile, newRestartCombine);
	atoms(Toluene_restartFile, Thiol_restartFile, newRestartCombine);
	/*velocities(Toluene_restartFile, Thiol_restartFile, newRestartCombine);
	bonds(Toluene_restartFile, Thiol_restartFile, newRestartCombine);
	angles(Toluene_restartFile, Thiol_restartFile, newRestartCombine);
	dihedrals(Toluene_restartFile, Thiol_restartFile, newRestartCombine);
	*/
}

//print everything until atoms section start from the old restart file into the new restart file//
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
void atoms(char *tolueneFile, char *thiolFile, char *newRestart){

	FILE *restartToluene = fopen(tolueneFile, "rt");
	FILE *restartThiol = fopen(thiolFile, "rt");
	FILE *output = fopen(newRestart, "a+");

	char line[200000];
	bool atomEnd = false;
	int section = 0;
	int atomCount = 0;
	int newAtomCount = 0;
		
	while( (fgets(line, 200000, restartToluene) != NULL )) {
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
		int moleculeid;
		int atomtype;
		float charge;
		float xcoor;
		float ycoor;
		float zcoor;
		int value1;
		int value2;
		int value3;
		
		int scan = sscanf(line, "%d %d %d %f %f %f %f %d %d %d", &atomid, &moleculeid, &atomtype, &charge, &xcoor, &ycoor, &zcoor, &value1, &value2, &value3);
		
		if(scan < 10){
			break;
		}

		if(atomtype < 5){
			newAtomCount++;
			atom[atomid] = toluene;
			fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", atomid, moleculeid, atomtype, charge, xcoor, ycoor, zcoor, value1, value2, value3);
		}
		else if(atomtype > 4){

			newAtomCount++;
			
			xcoor = xcoor - xcenThiol + xcenToluene;
			ycoor = ycoor - ycenThiol + ycenToluene;
			zcoor = zcoor - zcenThiol + zcenToluene;
		
			atom[atomid] = thiol;
			fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", (atomid + highestToluene), (moleculeid + highestToluene), atomtype, charge, xcoor, ycoor, zcoor, value1, value2, value3);
	
		}

		
		//fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", atomid, moleculeid, atomtype, charge, xcoor, ycoor, zcoor, value1, value2, value3);
	
		} //section > 2//

		//printf(line);

		//printf("Working\n");

	}//end of while loop

	while( (fgets(line, 200000, restartThiol) != NULL )) {
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
		int moleculeid;
		int atomtype;
		float charge;
		float xcoor;
		float ycoor;
		float zcoor;
		int value1;
		int value2;
		int value3;
		
		int scan = sscanf(line, "%d %d %d %f %f %f %f %d %d %d", &atomid, &moleculeid, &atomtype, &charge, &xcoor, &ycoor, &zcoor, &value1, &value2, &value3);
		
		if(scan < 10){
			break;
		}

		if(atomtype < 5){
			newAtomCount++;
			atom[atomid] = toluene;
			fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", atomid, moleculeid, atomtype, charge, xcoor, ycoor, zcoor, value1, value2, value3);
		}
		else if(atomtype > 4){

			newAtomCount++;
			
			xcoor = xcoor - xcenThiol + xcenToluene;
			ycoor = ycoor - ycenThiol + ycenToluene;
			zcoor = zcoor - zcenThiol + zcenToluene;
		
			atom[atomid] = thiol;
			fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", (atomid + highestToluene), (moleculeid + highestToluene), atomtype, charge, xcoor, ycoor, zcoor, value1, value2, value3);
	
		}

		
		//fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", atomid, moleculeid, atomtype, charge, xcoor, ycoor, zcoor, value1, value2, value3);
	
		} //section > 2//

		//printf(line);

		//printf("Working\n");

	}//end of while loop

	fprintf(output, "End");
	printf("Entered writeAtoms\n");
	printf("New count of atoms %d\n", newAtomCount);

	fclose(restartToluene);
	fclose(restartThiol);
	fclose(output);
}
