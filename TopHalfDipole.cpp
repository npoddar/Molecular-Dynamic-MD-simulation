#include <stdio.h>
#include <math.h>
#include <string.h>

#define xhigh 170
#define xlow 154
#define yhigh 100
#define ylow 100
#define zhigh 315
#define zlow 205

int mol[404595];
int moldist[30000];

void molecules();


int main(){
	
	molecules();

}

void molecules(){
	FILE *input = fopen("coordinates.5600000.toluene", "rt");
	FILE *output = fopen("output.molecules.txt", "w+");
	FILE *outputd = fopen("output_d.molecules.txt", "w+");
	char line[200000];
	int molct = 0;

	while( (fgets(line, 200000, input) != NULL ) ) {
		int atomid;
		int moleculeid;
		
		float xcoor;
		float ycoor;
		float zcoor;

		sscanf(line, "%d %d %f %f %f", &atomid, &moleculeid, &xcoor, &ycoor, &zcoor);

		if( (zcoor > zlow) && (zcoor < zhigh) ) {
			mol[molct] = moleculeid;
			molct++;
		}

	}//end of while loop


	//calculating distinct molecules
	moldist[0] = mol[0];
	int dist = 0;

	for(int i = 0; i < molct; i++) {
		if(moldist[dist] != mol[i]){
			dist++;
			moldist[dist] = mol[i];
		}

	}

	//output to files//
	for(int i = 0; i < molct; i++){
		fprintf(output, "%d\n", mol[i]);
	}

	for(int i = 0; i < dist+1; i++){
		fprintf(outputd, "%d \n", moldist[i]);
	}

	//printing out no of molecules in region
	printf("No of atom in block region %d\n", molct);
	printf("No of molecules %d\n", dist);

	//close files
	fclose(input);
	fclose(output);
	fclose(outputd);

}

