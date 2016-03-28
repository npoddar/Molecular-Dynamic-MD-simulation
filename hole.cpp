#include <stdio.h>
#include <math.h>
#include <string.h>

#define xylow 120.3
#define xyhigh 218.3
#define zlow 261
#define zhigh 359

int mol[200000];
int moldist[1000];

void molecules();

int main(){
	
	molecules();

}

void molecules(){
	FILE *input = fopen("sort.toluene.250k", "rt");
	FILE *output = fopen("output.txt", "w+");
	FILE *outputd = fopen("output_d.txt", "w+");
	char line[200000];
	int molct = 0;

	while( (fgets(line, 200000, input) != NULL ) ) {
		int atomid;
		int moleculeid;
		int type;
		float xcoor;
		float ycoor;
		float zcoor;

		sscanf(line, "%d %d %d %f %f %f", &atomid, &moleculeid, &type, &xcoor, &ycoor, &zcoor);

		if( (xcoor > xylow) && (xcoor < xyhigh) && (ycoor > xylow) && (ycoor < xyhigh) && (zcoor > zlow) && (zcoor < zhigh) ) {
			mol[molct] = moleculeid;
			molct++;
		}


	}
	
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
		fprintf(outputd, "%d ", moldist[i]);
	}

	printf("No of atom in block region %d\n", molct);
	printf("No of molecules %d\n", dist);
	fclose(input);
	fclose(output);
	fclose(outputd);

}
