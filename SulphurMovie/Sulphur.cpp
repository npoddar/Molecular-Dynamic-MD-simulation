#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

void fileWriter(char *, char *);

int main(int argc, char *argv[]){
	FILE *file;
	FILE *output;
	char  *outputfilename;
	
	if(argc == 2) {
		file = fopen (argv[1], "rt");
		output = fopen(argv[2], "w+");
		outputfilename = argv[2];
	}

	else {
		file = fopen ("movieFiles.txt", "rt");
		output = fopen("movieSulphur", "w+");
		outputfilename = "movieSulphur";
	}


	char fileLine[2000000];

	printf("Time - 10:45pm \n");
	
	while( (fgets(fileLine, 2000000, file) != NULL) ){

		char *fileName = "";

		fileName = (char*) malloc (200000000);
		
		sscanf(fileLine, "%s", fileName);
		
		fileWriter(fileName, outputfilename);
		
		//fileName = "Hello";

		//printf("%s \n", fileName);


	}




	fclose(file);
	fclose(output);

}


void fileWriter(char *inputfileName, char *outputfileName){
	
	//printf("In FileWriter. ");
	//printf("Inputfile name %s, Outputfile name %s\n", inputfileName, outputfileName);	
	
	FILE *input = fopen(inputfileName, "rt");
	FILE *output = fopen(outputfileName, "a+");

	char line[2000000];
	int i = 0;

	while( (fgets(line, 2000000, input)) && (i < 7) ) {
		
		//char test1[20000], test2[20000], test3[20000], test4[20000];
		//int scan = sscanf(line, "%s %s %s %s", test1, test2, test3, test4);

		int a, b, c, d;
		int scan = sscanf(line, "%d %d %d %d", a, b, c, d);		
		
		fprintf(output, "Scan = %d\n", scan);		
		
		i++;
		
		
	}

	fclose(input);
	fclose(output);
	
}