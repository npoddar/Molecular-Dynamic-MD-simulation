#include <stdio.h>
#include <math.h>
#include <string.h>

void atoms();
void velocities();
void bonds();
void angles();
void dihedrals();

int main(){
	atoms();
	velocities();
	bonds();
	angles();
	dihedrals();
}

void atoms(){
	FILE *toluene = fopen("restart.toluene.250k.atoms.txt", "rt");
	FILE *thiol = fopen("restart.300k.thiol.atom.output", "rt");
	FILE *output = fopen("restart.combine", "a+");

	char line[2000000];
	int counter = 0;
	
	fprintf(output, "\n\nAtoms\n\n");
	
	while(fgets(line, 2000000, toluene) != NULL) {
		counter++;
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
		
		sscanf(line, "%d %d %d %f %f %f %f %d %d %d", &atomid, &moleculeid, &atomtype, &charge, &xcoor, &ycoor, &zcoor, &value1, &value2, &value3);
		
		fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", atomid, moleculeid, atomtype, charge, xcoor, ycoor, zcoor, value1, value2, value3);
	}
	
	while(fgets(line, 2000000, thiol) != NULL) {
		counter++;
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
		
		sscanf(line, "%d %d %d %f %f %f %f %d %d %d", &atomid, &moleculeid, &atomtype, &charge, &xcoor, &ycoor, &zcoor, &value1, &value2, &value3);
		
		fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", atomid, moleculeid, atomtype, charge, xcoor, ycoor, zcoor, value1, value2, value3);
	}

	printf("No of atoms : %d\n", counter);
	
	fclose(toluene);
	fclose(thiol);
	fclose(output);	
}

void velocities(){
	FILE *toluene = fopen("restart.toluene.250k.velocities.txt", "rt");
	FILE *thiol = fopen("restart.300k.thiol.velocity.output", "rt");
	FILE *output = fopen("restart.combine", "a+");
	
	char line[2000000];
	int counter = 0;
	
	fprintf(output, "\nVelocities\n\n");
	
	while(fgets(line, 2000000, toluene) != NULL) {
		counter++;
		int atomid;
		float xvel;
		float yvel;
		float zvel;
		
		sscanf(line, "%d %f %f %f", &atomid, &xvel, &yvel, &zvel);
		fprintf(output, "%d %f %f %f\n", atomid, xvel, yvel, zvel);
				
	}
	
	while(fgets(line, 2000000, thiol) != NULL) {
		counter++;
		int atomid;
		float xvel;
		float yvel;
		float zvel;
		
		sscanf(line, "%d %f %f %f", &atomid, &xvel, &yvel, &zvel);
		fprintf(output, "%d %f %f %f\n", atomid, xvel, yvel, zvel);
				
	}
	
	printf("No of velocity atoms : %d\n", counter);
	
	fclose(toluene);
	fclose(thiol);
	fclose(output);
}

void bonds(){
	FILE *toluene = fopen("restart.toluene.250k.bonds.txt", "rt");
	FILE *thiol = fopen("restart.300k.thiol.bond.output", "rt");
	FILE *output = fopen("restart.combine", "a+");
	
	char line[2000000];
	int counter = 0;
	
	fprintf(output, "\nBonds\n\n");
	
	while(fgets(line, 2000000, toluene) != NULL) {
		
		counter++;
		int bondid;
		int bondtype;
		int atom1;
		int atom2;
		
		sscanf(line, "%d %d %d %d", &bondid, &bondtype, &atom1, &atom2);
		fprintf(output, "%d %d %d %d\n", bondid, bondtype, atom1, atom2);

	}
	
	while(fgets(line, 2000000, thiol) != NULL) {
		
		counter++;
		int bondid;
		int bondtype;
		int atom1;
		int atom2;
		
		sscanf(line, "%d %d %d %d", &bondid, &bondtype, &atom1, &atom2);
		fprintf(output, "%d %d %d %d\n", bondid, bondtype, atom1, atom2);

	}

	printf("No of bonds : %d\n", counter);
	
	fclose(toluene);
	fclose(thiol);
	fclose(output);

}

void angles(){

	FILE *toluene = fopen("restart.toluene.250k.angle.txt", "rt");
	FILE *thiol = fopen("restart.300k.thiol.angle.output", "rt");
	FILE *output = fopen("restart.combine", "a+");

	char line[2000000];
	int counter = 0;
	
	fprintf(output, "\nAngles\n\n");
	
	while(fgets(line, 2000000, toluene) != NULL) {
		counter++;
		int angleid;
		int angletype;
		int atom1;
		int atom2;
		int atom3;
		
		sscanf(line, "%d %d %d %d %d", &angleid, &angletype, &atom1, &atom2, &atom3);
		fprintf(output, "%d %d %d %d %d\n", angleid, angletype, atom1, atom2, atom3);

	}
	
	while(fgets(line, 2000000, thiol) != NULL) {
		counter++;
		int angleid;
		int angletype;
		int atom1;
		int atom2;
		int atom3;
		
		sscanf(line, "%d %d %d %d %d", &angleid, &angletype, &atom1, &atom2, &atom3);
		fprintf(output, "%d %d %d %d %d\n", angleid, angletype, atom1, atom2, atom3);

	}
	
	printf("No of angles : %d\n", counter);
	
	fclose(toluene);
	fclose(thiol);
	fclose(output);
	
	
}

void dihedrals(){

	FILE *toluene = fopen("restart.toluene.250k.dihedrals.txt", "rt");
	FILE *thiol = fopen("restart.300k.thiol.dihedrals.output", "rt");
	FILE *output = fopen("restart.combine", "a+");
	
	char line[2000000];
	int counter = 0;
	
	fprintf(output, "\nDihedrals\n\n");

	while(fgets(line, 2000000, toluene) != NULL) {
		counter++;
		int dihedralid;
		int dihedraltype;
		int atom1;
		int atom2;
		int atom3;
		int atom4;
	
		sscanf(line, "%d %d %d %d %d %d", &dihedralid, &dihedraltype, &atom1, &atom2, &atom3, &atom4);
		
		fprintf(output, "%d %d %d %d %d %d\n", dihedralid, dihedraltype, atom1, atom2, atom3, atom4);

	}
	
	while(fgets(line, 2000000, thiol) != NULL) {
		counter++;
		int dihedralid;
		int dihedraltype;
		int atom1;
		int atom2;
		int atom3;
		int atom4;
	
		sscanf(line, "%d %d %d %d %d %d", &dihedralid, &dihedraltype, &atom1, &atom2, &atom3, &atom4);
		
		fprintf(output, "%d %d %d %d %d %d\n", dihedralid, dihedraltype, atom1, atom2, atom3, atom4);

	}
	
	printf("No of dihedrals : %d\n", counter);
	
	fclose(thiol);
	fclose(output);
	fclose(toluene);
	
}
