#include <stdio.h>
#include <math.h>
#include <string.h>

#define atomShift 405000 
#define moleculeShift 27000
#define bondShift 405000 
#define angleShift 648000 
#define dihedralShift 810000 

#define atomTypeShift 4
#define bondTypeShift 4
#define angleTypeShift 5
#define dihedralTypeShift 6

#define oldxcenter 0.007724
#define oldycenter 0.809344
#define oldzcenter -0.693567


#define newxcenter 169.3
#define newycenter 169.3
#define newzcenter 310

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
	FILE *input = fopen("thiol.300k.atom.txt", "rt");
	FILE *output = fopen("restart.300k.thiol.atom.output", "w+");
	
	//fprintf(output, "%s", "Atoms\n\n");
	char line[2000000];
	int counter = 0;
	
	while(fgets(line, 2000000, input) != NULL) {
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
		
		atomid = atomid + atomShift;
		moleculeid = moleculeid + moleculeShift;
		atomtype = atomtype + atomTypeShift;
		
		if( (value1 != 0) && (value2 != 0) && (value3 != 0) ) {
			printf("Not all image bits are zero");	
		}

		xcoor = xcoor - oldxcenter + newxcenter;
		ycoor = ycoor - oldycenter + newycenter;
		zcoor = zcoor - oldzcenter + newzcenter;
		
		fprintf(output, "%d %d %d %f %f %f %f %d %d %d\n", atomid, moleculeid, atomtype, charge, xcoor, ycoor, zcoor, value1, value2, value3);
		
		
	}
	
	printf("No of atoms : %d\n", counter);
	
	fclose(input);
	fclose(output);	
	
}

void velocities(){
	FILE *input = fopen("thiol.300k.velocity.txt", "rt");
	FILE *output = fopen("restart.300k.thiol.velocity.output", "w+");
	
	//fprintf(output, "%s", "Velocities\n\n");
	char line[2000000];
	int counter = 0;
	
	while(fgets(line, 2000000, input) != NULL) {
		counter++;
		int atomid;
		float xvel;
		float yvel;
		float zvel;
		
		sscanf(line, "%d %f %f %f", &atomid, &xvel, &yvel, &zvel);
		atomid = atomid + atomShift;
		
		fprintf(output, "%d %f %f %f\n", atomid, xvel, yvel, zvel);

	}
	
	printf("No of velocity atoms : %d\n", counter);
		
	fclose(input);
	fclose(output);
	
}

void bonds(){
	FILE *input = fopen("thiol.300k.bond.txt", "rt");
	FILE *output = fopen("restart.300k.thiol.bond.output", "w+");
	
	//fprintf(output, "%s", "Bonds\n\n");
	char line[2000000];
	int counter = 0;
	
	while(fgets(line, 2000000, input) != NULL) {
		counter++;
		int bondid;
		int bondtype;
		int atom1;
		int atom2;
		
		
		sscanf(line, "%d %d %d %d", &bondid, &bondtype, &atom1, &atom2);
		
		bondid = bondid + bondShift;
		bondtype = bondtype + bondTypeShift;
		atom1 = atom1 + atomShift;
		atom2 = atom2 + atomShift;		
		
		fprintf(output, "%d %d %d %d\n", bondid, bondtype, atom1, atom2);

	}
	
	printf("No of bonds : %d\n", counter);
	
	fclose(input);
	fclose(output);
	
}

void angles(){
	FILE *input = fopen("thiol.300k.angle.txt", "rt");
	FILE *output = fopen("restart.300k.thiol.angle.output", "w+");
	
	//fprintf(output, "%s", "Angles\n\n");
	char line[2000000];
	int counter = 0;
	
	while(fgets(line, 2000000, input) != NULL) {
		counter++;
		int angleid;
		int angletype;
		int atom1;
		int atom2;
		int atom3;
		
		
		sscanf(line, "%d %d %d %d %d", &angleid, &angletype, &atom1, &atom2, &atom3);
		
		angleid = angleid + angleShift;
		angletype = angletype + angleTypeShift;
		atom1 = atom1 + atomShift;
		atom2 = atom2 + atomShift;
		atom3 = atom3 + atomShift;		
		
		fprintf(output, "%d %d %d %d %d\n", angleid, angletype, atom1, atom2, atom3);

	}
	
	printf("No of angles : %d\n", counter);
	
	fclose(input);
	fclose(output);
	
}

void dihedrals(){
	FILE *input = fopen("thiol.300k.dihedrals.txt", "rt");
	FILE *output = fopen("restart.300k.thiol.dihedrals.output", "w+");
	
	//fprintf(output, "%s", "Dihedrals\n\n");
	char line[2000000];
	int counter = 0;
	
	while(fgets(line, 2000000, input) != NULL) {
		counter++;
		int dihedralid;
		int dihedraltype;
		int atom1;
		int atom2;
		int atom3;
		int atom4;
		
		
		sscanf(line, "%d %d %d %d %d %d", &dihedralid, &dihedraltype, &atom1, &atom2, &atom3, &atom4);
		
		dihedralid = dihedralid + dihedralShift;
		dihedraltype = dihedraltype + dihedralTypeShift;
		atom1 = atom1 + atomShift;
		atom2 = atom2 + atomShift;
		atom3 = atom3 + atomShift;
		atom4 = atom4 + atomShift;		
		
		fprintf(output, "%d %d %d %d %d %d\n", dihedralid, dihedraltype, atom1, atom2, atom3, atom4);

	}
	
	printf("No of dihedral : %d\n", counter);
	
	fclose(input);
	fclose(output);
	
}
