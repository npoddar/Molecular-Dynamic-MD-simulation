#include <stdio.h>
#include <math.h>
#include <string.h>

#define atomsAfterDel 425250
#define atomsBeforeDel 425520 

void consecutiveAtoms();
void bonds();
void angles();
void dihedrals();

int atomId[atomsBeforeDel+1];

int main(){
	
	consecutiveAtoms();
	bonds();	
	angles();
	dihedrals();

}

void consecutiveAtoms(){
	//Read atoms from dump file, and store its consecutive id number instead
	//index of array atomId[] is its actual id. The stored int is its consecutive id.

	FILE *input = fopen("dump.output", "rt");
	FILE *output = fopen("restart.250000.consecutive", "a+");

	fprintf(output, "Atoms\n\n");
	
	char line[2000000];
	int index = 1;

	while( (fgets(line, 2000000, input) != NULL ) ){

		int id, molid, type;
		float charge, xcoor, ycoor, zcoor;
		int imagex, imagey, imagez;

		sscanf(line, "%d %d %d %f %f %f %f %d %d %d", &id, &molid, &type, &charge, &xcoor, &ycoor, &zcoor, &imagex, &imagey, &imagez);
		
		atomId[id] = index;

		fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", index, molid, type, charge, xcoor, ycoor, zcoor, imagex, imagey, imagez);

		index++;
	}

	printf("Value of index is %d\n", index);

	fclose(input);
	fclose(output);
}

void bonds(){

	FILE *input = fopen("bonds.txt", "rt");
	FILE *output = fopen("restart.250000.consecutive", "a+");

	fprintf(output, "\nBonds\n\n");

	char line[2000000];

	while( (fgets(line, 2000000, input) != NULL ) ){

		int bondid, bondtype, id1, id2;

		sscanf(line, "%d %d %d %d", &bondid, &bondtype, &id1, &id2);

		fprintf(output, "%d %d %d %d\n", bondid, bondtype, atomId[id1], atomId[id2]);


	}//while loop

	fclose(input);
	fclose(output);

}

void angles(){

	FILE *input = fopen("angles.txt", "rt");
	FILE *output = fopen("restart.250000.consecutive", "a+");

	fprintf(output, "\nAngles\n\n");

	char line[2000000];

	while( (fgets(line, 2000000, input) != NULL ) ){

		int angleid, angletype, id1, id2, id3;

		sscanf(line, "%d %d %d %d %d", &angleid, &angletype, &id1, &id2, &id3);

		fprintf(output, "%d %d %d %d %d\n", angleid, angletype, atomId[id1], atomId[id2], atomId[id3]);


	}//while loop

	fclose(input);
	fclose(output);

}

void dihedrals(){

	FILE *input = fopen("dihedrals.txt", "rt");
	FILE *output = fopen("restart.250000.consecutive", "a+");

	fprintf(output, "\nDihedrals\n\n");

	char line[2000000];

	while( (fgets(line, 2000000, input) != NULL ) ){

		int angleid, angletype, id1, id2, id3, id4;

		sscanf(line, "%d %d %d %d %d %d", &angleid, &angletype, &id1, &id2, &id3, &id4);

		fprintf(output, "%d %d %d %d %d %d\n", angleid, angletype, atomId[id1], atomId[id2], atomId[id3], atomId[id4]);


	}//while loop

	fclose(input);
	fclose(output);

}